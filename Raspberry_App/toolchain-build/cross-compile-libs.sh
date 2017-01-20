#!/bin/bash

if [ $UID -ne 0 ]; then
	echo "must be run as root"
	exit 1
fi

# apt -y install lib32z1 libstdc++6:i386 git qemu-user proot cmake pkg-config

PIROOT=/mnt/rasp-pi-rootfs
BUILD_DIR=/tmp/build
TOOLCHAIN=gcc-linaro-4.9-2016.02-x86_64_arm-linux-gnueabihf
CMAKE_TOOLCHAIN_FILE=rpi-toolchain.cmake
##############################################################################################
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR

#download and unzip raspbian-img
wget -O raspbian.zip https://downloads.raspberrypi.org/raspbian_latest
unzip raspbian.zip
RASPBIAN_IMG=$(unzip -Z1 raspbian.zip)

#resize raspbian-img
LOOP_DEVICE=$(losetup -f)
RASPBIAN_PARTITION_BOOT=${LOOP_DEVICE}p1
RASPBIAN_PARTITION_SYSTEM=${LOOP_DEVICE}p2
truncate -s +1G $RASPBIAN_IMG
losetup $LOOP_DEVICE $RASPBIAN_IMG
parted $LOOP_DEVICE resizepart 2 100%
e2fsck -f $RASPBIAN_PARTITION_SYSTEM
resize2fs $RASPBIAN_PARTITION_SYSTEM

#mount raspbian-img
mkdir -p $PIROOT
umount $RASPBIAN_PARTITION_SYSTEM
umount $RASPBIAN_PARTITION_BOOT
mount $RASPBIAN_PARTITION_SYSTEM $PIROOT
mount $RASPBIAN_PARTITION_BOOT $PIROOT/boot

#fix absolute links
proot -q qemu-arm -S $PIROOT<<EOF
apt update

apt -y install libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev \
libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
libxvidcore-dev libx264-dev \
libgtk2.0-dev libatlas-base-dev gfortran \
libboost-all-dev libssl-dev

apt -y install symlinks
symlinks -cr /usr/lib
EOF

#cmake and make need this for pkg-config
export PKG_CONFIG_SYSROOT_DIR=${PIROOT}
export PKG_CONFIG_LIBDIR=${PIROOT}/usr/lib/arm-linux-gnueabihf/pkgconfig:${PIROOT}/usr/share/pkgconfig:${PIROOT}/usr/lib/pkgconfig
export PKG_CONFIG_DIR=

#opencv
wget -O opencv.zip https://github.com/opencv/opencv/archive/3.2.0.zip
unzip opencv.zip
cd opencv-*
mkdir build
cd build

cmake -D CMAKE_TOOLCHAIN_FILE="$CMAKE_TOOLCHAIN_FILE" -D PIROOT="$PIROOT" -D TOOLCHAIN="$TOOLCHAIN" -D CMAKE_BUILD_TYPE=Release -D WITH_V4L=ON -D WITH_OPENGL=ON -D ENABLE_PRECOMPILED_HEADERS=OFF ..
make -j $(nproc)
make install

#restsdk
cd $BUILD_DIR
git clone https://github.com/Microsoft/cpprestsdk.git
cd cpprestsdk/Release
mkdir build
cd build

cmake -D CMAKE_TOOLCHAIN_FILE="$CMAKE_TOOLCHAIN_FILE" -D PIROOT="$PIROOT" -D TOOLCHAIN="$TOOLCHAIN" -D CMAKE_BUILD_TYPE=Release ..
make -j $(nproc)
make install DESTDIR=$PIROOT

#wiringPi
cd $BUILD_DIR
git clone git://git.drogon.net/wiringPi
cd wiringPi/wiringPi

cat > CMakeLists.txt<<EOF
project(wiringPi)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
list(APPEND CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

aux_source_directory(. SRC_LIST)
add_library(\${PROJECT_NAME} SHARED \${SRC_LIST})

install (TARGETS \${PROJECT_NAME} DESTINATION lib)
install (FILES \${PROJECT_NAME}.h DESTINATION include)
EOF

mkdir build
cd build
cmake -D CMAKE_TOOLCHAIN_FILE="$CMAKE_TOOLCHAIN_FILE" -D PIROOT="$PIROOT" -D TOOLCHAIN="$TOOLCHAIN" ..
make -j $(nproc)
make install DESTDIR=$PIROOT

#settings
echo bcm2835-v4l2 >> $PIROOT/etc/modules #enable camera v4l2 driver
touch $PIROOT/boot/ssh #enable ssh
sed -i 's/#xserver-command=X/xserver-command=X -s 0 -dpms/g' $PIROOT/etc/lightdm/lightdm.conf #disable screensaver
echo "export XAUTHORITY=/home/pi/.Xauthority" >> $PIROOT/etc/profile #allow root access to xserver

#enable camera
cat >> $PIROOT/boot/config.txt <<EOF
start_x=1
gpu_mem=128
EOF

proot -q qemu-arm -S $PIROOT<<EOF
apt update
apt -y install dnsmasq hostapd
systemctl enable hostapd
systemctl enable dnsmasq
EOF

####ACCESS POINT########
#ignore wlan0 interface
echo "denyinterfaces wlan0" >> $PIROOT/etc/dhcpcd.conf

#assign static ip for wlan0
cat > $PIROOT/etc/network/interfaces<<EOF
source-directory /etc/network/interfaces.d

auto lo
iface lo inet loopback

iface eth0 inet manual

allow-hotplug wlan0
iface wlan0 inet static
    address 10.5.0.1
    netmask 255.255.255.0
    network 10.5.0.0
    broadcast 10.5.0.255
EOF

#hostapd
cat > $PIROOT/etc/hostapd/hostapd.conf<<EOF
interface=wlan0
driver=nl80211
ssid=Pi3-AP
hw_mode=g
channel=6
ieee80211n=1
wmm_enabled=1
ht_capab=[HT40][SHORT-GI-20][DSSS_CCK-40]
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_key_mgmt=WPA-PSK
wpa_passphrase=workstation10
rsn_pairwise=CCMP
EOF
echo "DAEMON_CONF=\"/etc/hostapd/hostapd.conf\"" >> $PIROOT/etc/default/hostapd

#dnsmasq
mv $PIROOT/etc/dnsmasq.conf $PIROOT/etc/dnsmasq.conf.orig
cat > $PIROOT/etc/dnsmasq.conf<<EOF
interface=wlan0
listen-address=10.5.0.1
bind-interfaces
server=8.8.8.8
domain-needed
bogus-priv
dhcp-range=10.5.0.100,10.5.0.150,12h
EOF


cat > $PIROOT/home/pi/tl.sh<<EOF
#!/bin/bash

if [ \$UID -ne 0 ]; then
	sudo "\$0" "\$@"
	exit \$?
fi


export DISPLAY=:0
./TrafficLight
EOF
chmod +x $PIROOT/home/pi/tl.sh

sync; umount $PIROOT/boot $PIROOT
losetup -d $LOOP_DEVICE

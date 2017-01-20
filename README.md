# V2I-Traffic-Light-Demonstrator
V2I Traffic Light Demonstrator (vehicle-to-infrastructure)

## Raspberry / Traffic Light Application
Steps for building the traffic light application:

0. (Get a Debian based host system, we used Ubuntu 16.04)
1. Get the Linaro toolchain for ARM [gcc-linaro-4.9-2016.02-x86_64_arm-linux-gnueabihf](https://releases.linaro.org/components/toolchain/binaries/4.9-2016.02/arm-linux-gnueabihf/gcc-linaro-4.9-2016.02-x86_64_arm-linux-gnueabihf.tar.xz)
2. Extract it somewhere
3. Look into the build script *Raspberry_App/toolchain-build/cross-compile-libs.sh*
4. It has no error checking, so it's not meant to be run as a whole.
5. Adjust the parameters at the beginning according to your system and run the commands one by one.
6. You should now have a Raspbian image in /tmp. Store it somewhere, where it won't get deleted
7. Use *Raspberry_App/toolchain-build/mount-raspbian.sh* to mount it
8. Use CMake for building:
```
cd Raspberry_App/TrafficLight
mkdir build && cd build
cmake -D CMAKE_TOOLCHAIN_FILE="/path/to/rpi-toolchain.cmake" -D PIROOT="/path/to/rasp-pi-rootfs" -D TOOLCHAIN="/path/to/gcc-linaro-4.9-2016.02-x86_64_arm-linux-gnueabihf" ..
make
```

#!/bin/bash

if [ $# -ne 1 ]; then
    echo $0: usage: raspbian.img
    exit 1
fi

if [ $UID -ne 0 ]; then
	echo "must be run as root"
	exit 1
fi

RASPI_ROOT="/mnt/rasp-pi-rootfs"

if [ ! -d "$RASPI_ROOT" ]; then
	mkdir /mnt/rasp-pi-rootfs
elif [ "$(ls -A "$RASPI_ROOT")" ]; then
	echo "raspbian mounted. exiting..."
    exit 1
fi


if [ ! -d "$RASPI_ROOT" ]; then mkdir $RASPI_ROOT;fi

RASPBIAN_IMG=$1
SECTOR_OFFSET=$(fdisk -l $RASPBIAN_IMG | awk '$7 == "Linux" { print $2 }')
BYTE_OFFSET=$(expr 512 \* $SECTOR_OFFSET)

echo mount $RASPBIAN_IMG to $RASPI_ROOT
mount -o ro,loop,offset=$BYTE_OFFSET $RASPBIAN_IMG $RASPI_ROOT

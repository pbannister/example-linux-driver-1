#!/bin/sh
#
#	(Re-)load device driver kernel module.
#	Create device /dev/* tree.
#

MODULE_NAME=example1
MODULE_FILE=$MODULE_NAME.ko
DEVNAME=exampleA

echo "
==== Check that we can sudo."
sudo echo "We can." || {
    echo "ERROR cannot sudo!"
    exit 1
}

DEVPATH=/dev/$DEVNAME

echo "
==== Remove any existing $DEVPATH names."
test -e $DEVPATH || sudo mkdir -p $DEVPATH
sudo rm -rf $DEVPATH/*

echo "
==== Ensure driver module unloaded."
sudo rmmod $MODULE_FILE && echo "Module unloaded."

echo "
==== Ensure module is built."
make || {
    echo "ERROR cannot build module!"
    exit 1
}
sudo modinfo $MODULE_FILE

echo "
==== Load module with specified device name."
sudo insmod $MODULE_FILE name=$DEVNAME buffer_mb=4 || {
    echo "ERROR cannot load module!"
    exit 1
}
SYSPATH=/sys/module/$MODULE_NAME
find $SYSPATH/parameters -type f | xargs ls -l

# Allow for dynamically allocated major device number.
MAJOR=$( cat $SYSPATH/parameters/major )

echo "
==== Create $DEVPATH nodes for device major: $MAJOR ."
sudo mknod -m 0666 $DEVPATH/control  c $MAJOR 0
sudo mknod -m 0666 $DEVPATH/request  c $MAJOR 1
sudo mknod -m 0666 $DEVPATH/response c $MAJOR 2
sudo mknod -m 0666 $DEVPATH/antenna1 c $MAJOR 3
sudo mknod -m 0666 $DEVPATH/antenna2 c $MAJOR 4
sudo mknod -m 0666 $DEVPATH/antenna3 c $MAJOR 5
sudo mknod -m 0666 $DEVPATH/antenna4 c $MAJOR 6
ls -l $DEVPATH/*

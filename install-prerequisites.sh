#!/bin/bash

echo "
===== Check we can sudo."
sudo echo "  ... We can." || {
    echo "ERROR cannot sudo!"
    exit 1
}

echo "
===== Make sure we are up-to-date."
sudo apt update || {
    echo "ERROR cannot update from repositiories!"
    exit 1
}

echo "
===== Install driver build prerequisites."
sudo apt install -y build-essential linux-headers-`uname -r` || {
    echo "ERROR cannot install driver build packages!"
    exit 1
}

echo "
===== Install Linux headers for current kernel."
sudo aptcache search linux-headers-$(uname -r) | 
    awk '{print $1}' | 
    sudo xargs apt install -y || {
    echo "ERROR cannot install Linux headers for kernel!"
    exit 1
}

#
# How to find compiler include directories:
#   echo | gcc -xc -E -v -
# Needs to be executed in context of Makefile.
#

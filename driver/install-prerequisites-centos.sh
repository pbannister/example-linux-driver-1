#!/bin/bash

echo "
===== Check we can sudo."
sudo echo "  ... We can." || {
    echo "ERROR cannot sudo!"
    exit 1
}

echo "
===== Make sure we are up-to-date."
sudo yum update || {
    echo "ERROR cannot update from repositiories!"
    exit 1
}

echo "
===== Install development tools."
sudo yum groupinstall -y 'Development Tools' || {
    echo "ERROR cannot install development tools!"
    exit 1
}

echo "
===== Install driver build prerequisites."
sudo yum install -y elfutils-libelf-devel kernel-devel || {
    echo "ERROR cannot install driver build packages!"
    exit 1
}


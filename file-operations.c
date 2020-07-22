#include <linux/fs.h>
#include "common.h"

enum MINOR_DEVICES {
    MINOR_CONTROL = 0,
    MINOR_REQUEST = 1,
    MINOR_RESPONSE = 2,
    MINOR_STATUS = 3,
    MINOR_ANTENNA1 = 4,
    MINOR_ANTENNA2 = 5,
    MINOR_ANTENNA3 = 6,
    MINOR_ANTENNA4 = 7,
    MINOR_LIMIT = 8
};

static int device_open(struct inode* p_inode, struct file* p_file) {
	int minor = iminor(p_inode);
	switch (minor) {
    case MINOR_CONTROL:
        return 0;
    case MINOR_REQUEST:
        return 0;
    case MINOR_RESPONSE:
        return 0;
    case MINOR_STATUS:
        return 0;
    case MINOR_ANTENNA1:
        return 0;
    case MINOR_ANTENNA2:
        return 0;
    case MINOR_ANTENNA3:
        return 0;
    case MINOR_ANTENNA4:
        return 0;
    }
	printk(LOG_INFO "Device minor: %3d inode: %px file: %px -- OPEN\n", minor, p_inode, p_file);
	return -EINVAL;
}

static int device_release(struct inode* p_inode, struct file* p_file) {
    return 0;
}

static int device_mmap(struct file* p_file, struct vm_area_struct* p_vm) {
	int minor = iminor(p_file->f_inode);
	switch (minor) {
    case MINOR_REQUEST:
        return 0;
    case MINOR_RESPONSE:
        return 0;
    case MINOR_ANTENNA1:
        return 0;
    case MINOR_ANTENNA2:
        return 0;
    case MINOR_ANTENNA3:
        return 0;
    case MINOR_ANTENNA4:
        return 0;
    }
    return -EINVAL;
}

static ssize_t device_read(struct file* p_file, char __user* p_user, size_t n_want, loff_t* p_offset) {
	int minor = iminor(p_file->f_inode);
	switch (minor) {
    case MINOR_CONTROL:
        return 0;
    case MINOR_STATUS:
        return 0;
    }
    return -EINVAL;
}

static ssize_t device_write(struct file* p_file, const char __user* p_user, size_t n_want, loff_t* p_offset) {
	int minor = iminor(p_file->f_inode);
	switch (minor) {
    case MINOR_CONTROL:
        return 0;
    case MINOR_STATUS:
        return 0;
    }
    return -EINVAL;
}

struct file_operations driver_operations_g = {
    .open = device_open,
    .release = device_release,
    .mmap = device_mmap,
    .read = device_read,
    .write = device_write,
};

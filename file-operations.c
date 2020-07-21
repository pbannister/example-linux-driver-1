#include <linux/fs.h>

#include "common.h"

static int file_open(struct inode* p_inode , struct file* p_file) {
    return 0;
}

struct file_operations driver_operations_g = {
    .open = file_open,
    // .release = file_release,
    // .mmap = file_mmap,

};

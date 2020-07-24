#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include <linux/dma-iommu.h>
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

static inline int file_has_flags(struct file* p_file, int flags) {
    return flags == (flags & p_file->f_flags);
}

//
//
//

static struct status_s {
    uint32_t version;
} status_driver = {
    .version = VERSION_ID_MODULE,
};

//
//
//

typedef uint32_t* uint32_p;

typedef struct mmap_buffer_s {
    size_t size_buffer;
    uint32_p p_buffer;
} mmap_buffer_t;

typedef mmap_buffer_t* mmap_buffer_p;

static inline bool mmap_buffer_allocate(mmap_buffer_p p) {
    p->p_buffer = kzalloc(p->size_buffer, GFP_KERNEL);
    return (0 != p->p_buffer);
}

static inline bool mmap_buffer_allocate_dma(mmap_buffer_p p) {
    p->p_buffer = kzalloc(p->size_buffer, GFP_KERNEL);
    return (0 != p->p_buffer);
}

static inline void mmap_buffer_free(mmap_buffer_p p) {
    if (p->p_buffer) {
        kzfree(p->p_buffer);
        p->p_buffer = 0;
    }
}

//
//
//

static mmap_buffer_t buffer_request;
static mmap_buffer_t buffer_response;
static mmap_buffer_t buffer_antenna1;
static mmap_buffer_t buffer_antenna2;
static mmap_buffer_t buffer_antenna3;
static mmap_buffer_t buffer_antenna4;

enum BUFFER_SIZES {
    BUFFER_SIZE_REQUEST = (1 << 16),
    BUFFER_SIZE_RESPONSE = (1 << 16),
};

// Fetch short name / static module parameter value.
extern unsigned example_parameter_buffer_mb_get(void);

//
//
//

static int device_open_control(struct file* p_file) {
    {
        // Module parameter may have changed.
        unsigned buffer_mb = example_parameter_buffer_mb_get();
        size_t size_buffer = buffer_mb;
        size_buffer <<= 20;
        printk(LOG_INFO "DMA buffer size (MB): %u\n", buffer_mb);
        // Specify actual buffer sizes.
        buffer_request.size_buffer = BUFFER_SIZE_REQUEST;
        buffer_response.size_buffer = BUFFER_SIZE_RESPONSE;
        buffer_antenna1.size_buffer = size_buffer;
        buffer_antenna2.size_buffer = size_buffer;
        buffer_antenna3.size_buffer = size_buffer;
        buffer_antenna4.size_buffer = size_buffer;
    }
    // Allocate buffers.
    if (!mmap_buffer_allocate(&buffer_request)) {
        printk(LOG_ERROR "Cannot allocate mmap memory for request!\n");
    } else if (!mmap_buffer_allocate(&buffer_response)) {
        printk(LOG_ERROR "Cannot allocate mmap memory for response!\n");
    } else if (!mmap_buffer_allocate_dma(&buffer_antenna1)) {
        printk(LOG_ERROR "Cannot allocate mmap memory for antenna1!\n");
    } else if (!mmap_buffer_allocate_dma(&buffer_antenna2)) {
        printk(LOG_ERROR "Cannot allocate mmap memory for antenna2!\n");
    } else if (!mmap_buffer_allocate_dma(&buffer_antenna3)) {
        printk(LOG_ERROR "Cannot allocate mmap memory for antenna3!\n");
    } else if (!mmap_buffer_allocate_dma(&buffer_antenna4)) {
        printk(LOG_ERROR "Cannot allocate mmap memory for antenna4!\n");
    } else {
        // All buffers allocated - success!
        printk(LOG_INFO "Size of request  buffer : %ld\n", buffer_request.size_buffer);
        printk(LOG_INFO "Size of response buffer : %ld\n", buffer_response.size_buffer);
        printk(LOG_INFO "Size of antenna1 buffer : %ld\n", buffer_antenna1.size_buffer);
        printk(LOG_INFO "Size of antenna2 buffer : %ld\n", buffer_antenna2.size_buffer);
        printk(LOG_INFO "Size of antenna3 buffer : %ld\n", buffer_antenna3.size_buffer);
        printk(LOG_INFO "Size of antenna4 buffer : %ld\n", buffer_antenna4.size_buffer);
        return 0;
    }
    mmap_buffer_free(&buffer_request);
    mmap_buffer_free(&buffer_response);
    mmap_buffer_free(&buffer_antenna1);
    mmap_buffer_free(&buffer_antenna2);
    mmap_buffer_free(&buffer_antenna3);
    mmap_buffer_free(&buffer_antenna4);
    return -ENOMEM;
}

//
//
//

static int device_release_control(struct file* p_file) {
    mmap_buffer_free(&buffer_request);
    mmap_buffer_free(&buffer_response);
    mmap_buffer_free(&buffer_antenna1);
    mmap_buffer_free(&buffer_antenna2);
    mmap_buffer_free(&buffer_antenna3);
    mmap_buffer_free(&buffer_antenna4);
    return 0;
}

//
//
//

static int device_open(struct inode* p_inode, struct file* p_file) {
	int minor = iminor(p_inode);
	printk(LOG_INFO "Device minor: %3d inode: %px file: %px -- OPEN\n", minor, p_inode, p_file);
	switch (minor) {
    case MINOR_CONTROL:
        // if (!file_has_flags(p_file, O_RDWR)) {
        //     return -EINVAL;
        // }
        return device_open_control(p_file);
    case MINOR_REQUEST:
        if (!file_has_flags(p_file, O_RDWR)) {
            return -EINVAL;
        }
        return 0;
    case MINOR_RESPONSE:
        if (!file_has_flags(p_file, O_RDONLY)) {
            return -EINVAL;
        }
        return 0;
    case MINOR_STATUS:
        return 0;
    case MINOR_ANTENNA1:
        if (!file_has_flags(p_file, O_RDONLY)) {
            return -EINVAL;
        }
        return 0;
    case MINOR_ANTENNA2:
        if (!file_has_flags(p_file, O_RDONLY)) {
            return -EINVAL;
        }
        return 0;
    case MINOR_ANTENNA3:
        if (!file_has_flags(p_file, O_RDONLY)) {
            return -EINVAL;
        }
        return 0;
    case MINOR_ANTENNA4:
        if (!file_has_flags(p_file, O_RDONLY)) {
            return -EINVAL;
        }
        return 0;
    }
	return -EINVAL;
}

//
//
//

static int device_release(struct inode* p_inode, struct file* p_file) {
	int minor = iminor(p_inode);
    printk(LOG_INFO "Device minor: %3d inode: %px file: %px -- CLOSE\n", minor, p_inode, p_file);
	switch (minor) {
    case MINOR_CONTROL:
        return device_release_control(p_file);
    }
    return 0;
}

//
//  Note that none of the device file support non-zero offsets (by design).
//  File position is always zero (at start).
//  Seek to end does report file (buffer) size.
//

static loff_t device_seek(struct file* p_file, loff_t n_offset, int whence) {
	int minor = iminor(p_file->f_inode);
    p_file->f_pos = 0; // We do not use the file position.
    if (0 != n_offset) {
        // We do not support non-zero seek offsets.
        return -EINVAL;
    }
    if (SEEK_END != whence) {
        // We only support seek to end (to report size).
        return -EINVAL;
    }
	switch (minor) {
    case MINOR_CONTROL:
        return sizeof(uint32_t);
    case MINOR_REQUEST:
        return buffer_request.size_buffer;
    case MINOR_RESPONSE:
        return buffer_response.size_buffer;
    case MINOR_STATUS:
        return sizeof(status_driver);
    case MINOR_ANTENNA1:
        return buffer_antenna1.size_buffer;
    case MINOR_ANTENNA2:
        return buffer_antenna2.size_buffer;
    case MINOR_ANTENNA3:
        return buffer_antenna3.size_buffer;
    case MINOR_ANTENNA4:
        return buffer_antenna4.size_buffer;
    }
    return -EINVAL;
}

//
//
//

static int device_mmap(struct file* p_file, struct vm_area_struct* p_vm) {
	int minor = iminor(p_file->f_inode);
    printk(LOG_INFO "mmap start: %08lx end: %08lx\n", p_vm->vm_start, p_vm->vm_end);
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

enum CONTROL_REQUEST_STATUS {
    CONTROL_REQUEST_IDLE = 0,
    CONTROL_REQUEST_ACTIVE = 1,
    CONTROL_REQUEST_ERROR = 2,
    CONTROL_REQUEST_COMPLETE = 3,
};

//
//
//

static uint32_t device_read_control_value(struct file* p_file) {
    int request_started = 0;
    int request_complete = 0;
    int request_error = 0;
    // TODO determine if request in process.
    if (!request_started) {
        return CONTROL_REQUEST_IDLE;
    }
    // TODO determine if request complete.
    if (!request_complete) {
        return CONTROL_REQUEST_ACTIVE;
    }
    // TODO determine if request error.
    if (request_error) {
        return CONTROL_REQUEST_ERROR;
    }
    return CONTROL_REQUEST_COMPLETE;
}

//
//  The read(2) from control never blocks.
//  Returns status of request.
//

static ssize_t device_read_control(struct file* p_file, char __user* p_user, size_t n_want, loff_t* p_offset) {
    uint32_t value = CONTROL_REQUEST_IDLE;
    size_t n_need = sizeof(value);
    *p_offset = 0;
    if (n_need == n_want) {
        value = device_read_control_value(p_file);
        if (0 == put_user(value, p_user)) {
            return n_need;
        }
    }
    return -EINVAL;
}

//
//
//

static ssize_t device_read_status(struct file* p_file, char __user* p_user, size_t n_want, loff_t* p_offset) {
	return -EINVAL;
}

static ssize_t device_read(struct file* p_file, char __user* p_user, size_t n_want, loff_t* p_offset) {
	int minor = iminor(p_file->f_inode);
	switch (minor) {
    case MINOR_CONTROL:
        return device_read_control(p_file, p_user, n_want, p_offset);
    case MINOR_STATUS:
        return device_read_status(p_file, p_user, n_want, p_offset);
    }
    return -EINVAL;
}

//
//
//

static ssize_t device_write_control(struct file* p_file, const char __user* p_user, size_t n_want, loff_t* p_offset) {
    uint32_t command = 0;
    size_t n_need = sizeof(command);
    if (n_need == n_want) {
        if (0 == copy_from_user(&command, p_user, n_need)) {
            switch (command) {
            case 0:
                // Do nothing, just as test.
                return n_need;
            case 1:
                // TODO start request.
                return n_need;
            }
        }
    }
    return -EINVAL;
}

//
//
//

static ssize_t device_write_status(struct file* p_file, const char __user* p_user, size_t n_want, loff_t* p_offset) {
    return -EINVAL;
}

//
//
//

static ssize_t device_write(struct file* p_file, const char __user* p_user, size_t n_want, loff_t* p_offset) {
	int minor = iminor(p_file->f_inode);
	switch (minor) {
    case MINOR_CONTROL:
        return device_write_control(p_file, p_user, n_want, p_offset);
    case MINOR_STATUS:
        return device_write_status(p_file, p_user, n_want, p_offset);
    }
    return -EINVAL;
}

//
//
//

struct file_operations example_driver_operations_g = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .llseek = device_seek,
    .mmap = device_mmap,
    .read = device_read,
    .write = device_write,
};

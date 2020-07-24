#include "common.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

// Vendor code for PCI device.
#define VENDOR_ID   0x10EE  // Xilinx

// Module parameter for major device number.
//
// From Documentation/admin-guide/devices.txt:
//  60-63 char	LOCAL/EXPERIMENTAL USE
//
//static unsigned major = 60;
static unsigned major = 0;  // Dynamic allocation of major device number.
module_param(major, uint, 0444);    // Only set on module load.
MODULE_PARM_DESC(major, "Major device number");

// Device name given to Linux kernel.
static char* name = DEVICE_NAME;
module_param(name, charp, 0444);    // Only set on module load.
MODULE_PARM_DESC(name, "Device name as registered to kernel.");

// Size (in MB) for large DMA buffers.
static unsigned buffer_mb = 256;
module_param(buffer_mb, uint, 0644);
MODULE_PARM_DESC(buffer_mb, "Size (in MB) for large DMA buffers."); 

// Example parameters
static char* flags1 = "(none)";
static unsigned flags2 = 456;

module_param(flags1, charp, 0644);
module_param(flags2, uint, 0644);

MODULE_PARM_DESC(flags1, "Set of character flags.");
MODULE_PARM_DESC(flags2, "Set of bit flags.");

extern struct file_operations example_driver_operations_g;

//
//
//

unsigned example_parameter_buffer_mb_get(void) {
    return buffer_mb;
}

//
//
//

static int __init example_device_configure_pci(pci_dev_p p_pci_device) {
    // TODO
    return 0;
}

//
//
//

static int __init example_devices_configure(void) {
    if (0) {
        // Find *all* PCI devices with the given vendor ID.
        pci_dev_p p_pci_device = 0;
        p_pci_device = pci_get_device(VENDOR_ID, PCI_ANY_ID, p_pci_device);
        if (!p_pci_device) {
            printk(LOG_WARN "No PCI device found for ID %04x.\n", VENDOR_ID);
            return -ENODEV;
        }
        // If and only if one device expected.
        if (pci_get_device(VENDOR_ID, PCI_ANY_ID, p_pci_device)) {
            printk(LOG_ERROR "Second PCI device found - not a supported configuration!\n");
            return -ENODEV;
        }
        {
            int rc = example_device_configure_pci(p_pci_device);
            if (rc) {
                printk(LOG_ERROR "Cannot enable device!\n");
                return rc;
            }
        }
    }
    return 0;
}

//
//  Called when first loaded by Linux kernel.
//

static int __init example_driver_init(void) {
    printk(LOG_NOTE "Start %s driver for device %s initialization...\n", COMPANY_NAME, name);
    {
        int rc = example_devices_configure();
        if (rc) {
            return rc;
        }
    }
    {
        // Register/obtain the major device number.
        int v = register_chrdev(major, name, &example_driver_operations_g);
        if (v < 0) {
            printk(LOG_ERROR "Cannot register device name: %s major: %d!", name, major);
            return v;
        }
        if (0 == major) {
            major = v;
            printk(LOG_INFO "Device name: %s assigned major device number: %d", name, major);
        }
    }
    printk(LOG_INFO "Finish %s driver initialization.\n", COMPANY_NAME);
    return 0;
}

//
//
//

static void __exit example_devices_cleanup(void) {
    // TODO
}

//
//
//

static void __exit example_driver_exit(void) {
    printk(LOG_NOTE "Start %s driver cleanup...\n", COMPANY_NAME);
    // Tell the kernel our device class is no longer supported.
    unregister_chrdev(major, name);
    // Cleanup any active devices.
    example_devices_cleanup();
    printk(LOG_INFO "Finished %s driver cleanup.\n", COMPANY_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Preston L. Bannister <preston@bannister.us>");
MODULE_DESCRIPTION("Description of example driver.");
MODULE_VERSION(VERSION_OF_MODULE);
MODULE_SUPPORTED_DEVICE(DEVICE_NAME);

module_init(example_driver_init);
module_exit(example_driver_exit);

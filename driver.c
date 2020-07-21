
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "version.h"

#define LOG_NAME    "EXAMPLE1: "
#define LOG_ERR     KERN_ERR LOG_NAME 
#define LOG_WARNING KERN_WARNING LOG_NAME 
#define LOG_NOTICE  KERN_NOTICE LOG_NAME 
#define LOG_INFO    KERN_INFO LOG_NAME 
#define LOG_DEBUG   KERN_DEBUG LOG_NAME 

// static const char* driver_name = "ex1";
// static unsigned driver_number = 0;

// MODULE_PARM_DESC(driver_name, "Name displayed in kernel log.");
// MODULE_PARM_DESC(driver_number, "Number of no meaning.");

static int __init driver_init(void) {
    return 0;
}

static void __exit driver_exit(void) {
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Preston L. Bannister");
MODULE_DESCRIPTION("Example driver for FPGA firmware.");
MODULE_VERSION(VERSION_OF_MODULE);

module_init(driver_init);
module_exit(driver_exit);

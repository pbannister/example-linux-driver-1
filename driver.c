
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "common.h"

static char* flags1 = "(none)";
static unsigned flags2 = 123;

module_param(flags1, charp, 0644);
module_param(flags2, uint, 0644);
MODULE_PARM_DESC(flags1, "Set of character flags.");
MODULE_PARM_DESC(flags2, "Set of bit flags.");

extern struct file_operations driver_operations_g;

static int __init driver_init(void) {
    pr_alert("Loaded driver1 init.\n");
    return 0;
}

static void __exit driver_exit(void) {
    pr_alert("Unloaded driver1 exit.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Preston L. Bannister <preston@bannister.us>");
MODULE_DESCRIPTION("Description of example driver.");
MODULE_VERSION(VERSION_OF_MODULE);

module_init(driver_init);
module_exit(driver_exit);

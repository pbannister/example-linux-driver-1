#pragma once
#include <linux/pci.h>
#include "version.h"

typedef uint64_t tsc_t;
typedef struct pci_dev* pci_dev_p;

typedef struct active_device_s {
    pci_dev_p p_pci_device;
    // void __iomem* base0;
    // size_t size0;
} active_device_t;

typedef active_device_t* active_device_p;

#define COMPANY_NAME    "bannister.us"
#define DEVICE_NAME     "example1"

#define LOG_NAME        "EXAMPLE1: "
#define LOG_ERROR       KERN_ERR LOG_NAME 
#define LOG_WARN        KERN_WARNING LOG_NAME 
#define LOG_NOTE        KERN_NOTICE LOG_NAME 
#define LOG_INFO        KERN_INFO LOG_NAME 
#define LOG_DEBUG       KERN_DEBUG LOG_NAME 

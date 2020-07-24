#
#	Makefile for building a kernel module / device driver out of tree.
#
#	Use simple "make" to build the module. 
#
#	Use "make scrub" to compile with a higher level of compiler warnings.
#	(May generate false positives.)
#
#	Use "make bump" to re-build the driver with a current-date version.
#

#all : # default target (not used by kernel build)

# Wanted name of module.
TARGET 	:= example1

ifneq ($(KERNELRELEASE),)

#	Build rules needed by the kernel build system.
#	Invoked via a nested "make" during development.

ccflags-y := -march=native -mtune=native

# Use these options only to determine compiler include directories.
#ccflags-y := $(ccflags-y) -xc -E -v

# Called from kernel build system: desired target name for module.
obj-m := $(TARGET).o

# Called from kernel build system: declare files for module.
$(TARGET)-y := driver.o file-operations.o

else

#	Build rules used before invoking the kernel build system.
#	Support for typical development activities.

# Kernel version of target system (if development system not target).
K_EXPECT := 5.4.0-39-generic

# Kernel version of development system.
K_ACTUAL := $(shell uname -r)

# Build for the desired kernel version.
#K_RELEASE := $(K_EXPECT)
K_RELEASE := $(K_ACTUAL)

D_BUILD := /lib/modules/$(K_RELEASE)/build
D_PWD := $(shell pwd)

# Invoke kernel module build.
modules	: version.h	; $(MAKE) -C $(D_BUILD) M=$(D_PWD) modules
clean 	: 			; $(MAKE) -C $(D_BUILD) M=$(D_PWD) clean
help 	: 			; $(MAKE) -C $(D_BUILD) M=$(D_PWD) help

.PHONY: modules modules_install clean help

notice :
	@echo "Kernel build versions"
	@echo "    Expect : $(K_EXPECT)"
	@echo "    Actual : $(K_ACTUAL)"

all : notice modules

version.h: ; sh mkversion.sh
version.clean: ; -rm -f version.h
version.bump: version.clean version.h 

# Build the driver with a new/current version.
bump : clean version.bump modules

build : modules
rebuild : clean modules

# Turn on more aggressive compiler warnings.
scrub 	: clean version.bump ; $(MAKE) W=1 -C $(D_BUILD) M=$(D_PWD) modules

.PHONY: all build rebuild all scrub bump version.clean version.bump 

endif

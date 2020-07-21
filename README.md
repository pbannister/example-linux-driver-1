# Example FPGA board driver
Experiments with an(other) Linux driver. Focused on add-in FPGA boards.

With an FPGA, the firmware may not be loaded when Linux scans PCIe.
Early PCIe device enumeration by Linux might miss the device.

In the more usual case, you might want to use pci_register_driver().
This along with .probe allows device setup on first boot, or hotplug.

For a device using an FPGA, the firmware may not be loaded early on.
One approach is to load the driver well after boot, and have the driver
look for the device (rather than Linux looking for the driver).

In the second case you are going to use pci_get_device() to find the device
(or devices) meant to be associated with the driver.


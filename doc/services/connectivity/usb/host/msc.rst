.. _usb_host_msc:

USB host Mass Storage Class (MSC)
#################################

Overview
********

The USB host stack can enumerate Mass Storage Class devices that speak
Bulk-Only Transport (BOT) and SCSI transparent command set. Storage bringup
binds each LUN to the :ref:`SCSI mid-layer <scsi_api>`, probes the device,
and optionally registers a :c:func:`scsi_disk` volume for :ref:`disk_access_api`
and FatFs.

Typical data path:

.. code-block:: none

   Application / fs shell
           |
   disk_access / FatFs
           |
   scsi_disk (drivers/disk/scsi_disk.c)
           |
   SCSI mid-layer (subsys/scsi/)
           |
   USB MSC BOT transport (subsys/usb/host/)
           |
   UHC driver (e.g. snps,dwc3 xHCI)

Enable :kconfig:option:`CONFIG_USB_HOST_STACK`, a UHC driver for your board,
:kconfig:option:`CONFIG_USBH_MSC`, and :kconfig:option:`CONFIG_DISK_DRIVER_SCSI`
when using ``scsi_disk`` volumes.

Configuration
*************

Core options in ``subsys/usb/host/Kconfig``:

* :kconfig:option:`CONFIG_USBH_MSC` — MSC descriptor parsing and transport helpers
* :kconfig:option:`CONFIG_USBH_MSC_CLASS` — automatic class probe registration
* :kconfig:option:`CONFIG_USBH_MSC_AUTO_BRINGUP` — run storage bringup from class
  probe (disable when the application drives bringup after enumeration)
* :kconfig:option:`CONFIG_USBH_MSC_DISK` — register probed LUNs as ``scsi_disk``
  volumes
* :kconfig:option:`CONFIG_USBH_MSC_STRICT_PROBE` — apply MSC specification probe
  rules (device class 0, BBB protocol, bulk endpoints, etc.)

See also :ref:`scsi_api` for SCSI mid-layer and ``scsi_disk`` options.

Application integration
***********************

After a device reaches ``USB_STATE_CONFIGURED``, call
:c:func:`usbh_msc_storage_bringup` (or rely on
:kconfig:option:`CONFIG_USBH_MSC_AUTO_BRINGUP`) to bind BOT, probe SCSI, and
optionally attach ``scsi_disk``. Override weak notification hooks when needed:

* :c:func:`usbh_device_configured_notify` — enumeration complete
* :c:func:`usbh_device_removed_notify` — disconnect; unmount volumes and drop
  stale ``udev`` references

On disconnect, call :c:func:`usbh_msc_storage_teardown` (or use the sample flow)
before the host stack frees the device object.

Sample
******

:zephyr:code-sample:`host_msc` demonstrates enumeration, MSC bringup, FatFs mount
via the ``fs`` shell, and hot-unplug handling on :zephyr:board:`versal_apu` with
the ``snps,dwc3`` xHCI driver.

API reference
*************

Public headers:

* :file:`include/zephyr/usb/usbh_msc.h` — descriptor parsing and bringup API
* :file:`include/zephyr/usb/usbh_msc_bot.h` — BOT CBW/CSW helpers
* :file:`include/zephyr/usb/usb_msc_disk.h` — disk volume attach helpers

SCSI integration is documented in :ref:`scsi_api`.

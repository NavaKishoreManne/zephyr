/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief USB Chapter 9 request helpers for the host stack
 */

#ifndef ZEPHYR_INCLUDE_USBH_CH9_H
#define ZEPHYR_INCLUDE_USBH_CH9_H

#include <stdint.h>
#include <zephyr/usb/usbh.h>

#include "usbh_device.h"

/**
 * @brief Enable or disable omitting the control status stage on all requests.
 *
 * Mainly used for testing; driver support is optional.
 *
 * @param omit When true, skip the status stage for subsequent requests
 */
void usbh_req_omit_status(const bool omit);

/**
 * @brief Issue a raw USB control SETUP request.
 *
 * @param udev USB device
 * @param bmRequestType Setup packet bmRequestType
 * @param bRequest Setup packet bRequest
 * @param wValue Setup packet wValue
 * @param wIndex Setup packet wIndex
 * @param wLength Setup packet wLength
 * @param data Optional DATA stage buffer (may be NULL when @a wLength is 0)
 *
 * @retval 0 Request completed
 * @retval negative errno from control transfer
 */
int usbh_req_setup(struct usb_device *const udev, const uint8_t bmRequestType,
		   const uint8_t bRequest, const uint16_t wValue, const uint16_t wIndex,
		   const uint16_t wLength, struct net_buf *const data);

/**
 * @brief GET_DESCRIPTOR helper.
 *
 * @param udev USB device
 * @param type Descriptor type (high byte of wValue)
 * @param index Descriptor index (low byte of wValue)
 * @param id Language ID for string descriptors, otherwise 0
 * @param len Number of bytes to read
 * @param data Buffer for descriptor data
 *
 * @retval 0 Descriptor read succeeded
 * @retval negative errno from control transfer
 */
int usbh_req_desc(struct usb_device *const udev, const uint8_t type, const uint8_t index,
		  const uint16_t id, const uint16_t len, struct net_buf *const data);

/**
 * @brief GET_DESCRIPTOR(DEVICE) helper.
 *
 * @param udev USB device
 * @param len Number of bytes to read
 * @param dev Output device descriptor structure
 *
 * @retval 0 Descriptor read succeeded
 * @retval negative errno from control transfer
 */
int usbh_req_desc_dev(struct usb_device *const udev, const uint16_t len,
		      struct usb_device_descriptor *const dev);

/**
 * @brief GET_DESCRIPTOR(CONFIGURATION) helper.
 *
 * @param udev USB device
 * @param index Configuration index
 * @param len Number of bytes to read
 * @param desc Output configuration descriptor buffer
 *
 * @retval 0 Descriptor read succeeded
 * @retval negative errno from control transfer
 */
int usbh_req_desc_cfg(struct usb_device *const udev, const uint8_t index, const uint16_t len,
		      struct usb_cfg_descriptor *const desc);

/**
 * @brief SET_INTERFACE request.
 *
 * @param udev USB device
 * @param iface Interface number
 * @param alt Alternate setting number
 *
 * @retval 0 Alternate setting selected
 * @retval negative errno from control transfer
 */
int usbh_req_set_alt(struct usb_device *const udev, const uint8_t iface, const uint8_t alt);

/**
 * @brief SET_ADDRESS request.
 *
 * @param udev USB device
 * @param addr New device address (1–127)
 *
 * @retval 0 Address assigned
 * @retval negative errno from control transfer
 */
int usbh_req_set_address(struct usb_device *const udev, const uint8_t addr);

/**
 * @brief SET_CONFIGURATION request.
 *
 * @param udev USB device
 * @param cfg Configuration value from descriptor bConfigurationValue
 *
 * @retval 0 Configuration selected
 * @retval negative errno from control transfer
 */
int usbh_req_set_cfg(struct usb_device *const udev, const uint8_t cfg);

/**
 * @brief GET_CONFIGURATION request.
 *
 * @param udev USB device
 * @param cfg Output current configuration value
 *
 * @retval 0 Configuration read succeeded
 * @retval negative errno from control transfer
 */
int usbh_req_get_cfg(struct usb_device *const udev, uint8_t *const cfg);

/**
 * @brief SET_FEATURE(ENDPOINT_HALT) for a standard endpoint recipient.
 *
 * @param udev USB device
 * @param ep Full endpoint address including direction bit
 *
 * @retval 0 HALT set
 * @retval negative errno from control transfer
 */
int usbh_req_set_sfs_halt(struct usb_device *const udev, const uint8_t ep);

/**
 * @brief CLEAR_FEATURE(ENDPOINT_HALT) for a standard endpoint recipient.
 *
 * @param udev USB device
 * @param ep Full endpoint address including direction bit
 *
 * @retval 0 HALT cleared
 * @retval negative errno from control transfer
 */
int usbh_req_clear_sfs_halt(struct usb_device *const udev, const uint8_t ep);

/**
 * @brief SET_FEATURE(DEVICE_REMOTE_WAKEUP) for the device.
 *
 * @param udev USB device
 *
 * @retval 0 Remote wakeup enabled
 * @retval negative errno from control transfer
 */
int usbh_req_set_sfs_rwup(struct usb_device *const udev);

/**
 * @brief CLEAR_FEATURE(DEVICE_REMOTE_WAKEUP) for the device.
 *
 * @param udev USB device
 *
 * @retval 0 Remote wakeup disabled
 * @retval negative errno from control transfer
 */
int usbh_req_clear_sfs_rwup(struct usb_device *const udev);

#if IS_ENABLED(CONFIG_USBH_HUB)

/**
 * @brief GET_STATUS for a hub downstream port.
 *
 * @param hub Hub device
 * @param port 1-based port number
 * @param portstatus Output wPortStatus
 * @param portchange Output wPortChange
 *
 * @retval 0 Status read succeeded
 * @retval negative errno from control transfer
 */
int usbh_req_get_hcfs_port_status(struct usb_device *const hub, const uint8_t port,
				  uint16_t *const portstatus, uint16_t *const portchange);

/**
 * @brief GET_STATUS for the hub device.
 *
 * @param hub Hub device
 * @param hubstatus Output wHubStatus
 * @param hubchange Output wHubChange
 *
 * @retval 0 Status read succeeded
 * @retval negative errno from control transfer
 */
int usbh_req_get_hcfs_hub_status(struct usb_device *const hub, uint16_t *const hubstatus,
				 uint16_t *const hubchange);

/**
 * @brief SET_FEATURE on a hub downstream port.
 *
 * @param hub Hub device
 * @param port 1-based port number
 * @param feature Hub class port feature selector
 *
 * @retval 0 Feature set
 * @retval negative errno from control transfer
 */
int usbh_req_set_hcfs_port_feature(struct usb_device *const hub, const uint8_t port,
				   const uint8_t feature);

/**
 * @brief CLEAR_FEATURE on a hub downstream port.
 *
 * @param hub Hub device
 * @param port 1-based port number
 * @param feature Hub class port feature selector
 *
 * @retval 0 Feature cleared
 * @retval negative errno from control transfer
 */
int usbh_req_clear_hcfs_port_feature(struct usb_device *const hub, const uint8_t port,
				     const uint8_t feature);

/**
 * @brief CLEAR_FEATURE on the hub device.
 *
 * @param hub Hub device
 * @param feature Hub class hub feature selector
 *
 * @retval 0 Feature cleared
 * @retval negative errno from control transfer
 */
int usbh_req_clear_hcfs_hub_feature(struct usb_device *const hub, const uint8_t feature);

/**
 * @brief GET_DESCRIPTOR(HUB) helper.
 *
 * @param hub Hub device
 * @param len Number of bytes to read
 * @param desc Output buffer (variable-length hub descriptor)
 *
 * @retval 0 Descriptor read succeeded
 * @retval negative errno from control transfer
 */
int usbh_req_desc_hub(struct usb_device *const hub, const uint16_t len, void *const desc);

/**
 * @brief Issue hub port reset and clear C_PORT_RESET.
 *
 * @param hub Hub device
 * @param port 1-based port number
 * @param reset_ms Milliseconds to wait after SET_FEATURE(PORT_RESET)
 *
 * @retval 0 Port reset completed
 * @retval negative errno from control transfer
 */
int usbh_hub_port_reset(struct usb_device *const hub, const uint8_t port, const uint32_t reset_ms);

#endif /* CONFIG_USBH_HUB */

/**
 * @brief SET_FEATURE(PORT_POWER) on the root hub.
 *
 * @param udev USB device on the bus
 * @param port Root hub port number (1-based)
 *
 * @retval 0 Port power set
 * @retval negative errno from control transfer
 */
int usbh_req_set_hcfs_ppwr(const struct usb_device *udev, const uint8_t port);

/**
 * @brief SET_FEATURE(PORT_RESET) on the root hub.
 *
 * @param udev USB device on the bus
 * @param port Root hub port number (1-based)
 *
 * @retval 0 Port reset issued
 * @retval negative errno from control transfer
 */
int usbh_req_set_hcfs_prst(const struct usb_device *udev, const uint8_t port);

#endif /* ZEPHYR_INCLUDE_USBH_CH9_H */

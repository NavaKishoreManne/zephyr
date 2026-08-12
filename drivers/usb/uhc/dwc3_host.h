/* SPDX-License-Identifier: Apache-2.0
 *
 * Synopsys DWC3 core host-mode init.
 */

#ifndef ZEPHYR_USB_DWC3_HOST_H
#define ZEPHYR_USB_DWC3_HOST_H

#include "xhci_dwc3_priv.h"

int dwc3_host_burst_init(const struct uhc_dwc3_config *cfg);
void dwc3_host_susphy_post_run(const struct uhc_dwc3_config *cfg, bool enable);

#endif /* ZEPHYR_USB_DWC3_HOST_H */

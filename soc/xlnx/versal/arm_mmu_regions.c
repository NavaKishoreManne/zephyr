/*
 * Copyright (c) 2025 Advanced Micro Devices, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/arch/arm64/arm_mmu.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/util.h>

#define DEVICE_ATTR (MT_DEVICE_nGnRnE | MT_P_RW_U_NA | MT_DEFAULT_SECURE_STATE)

/* Covers VERSAL_USB2_IR_STATUS (0x64) plus access width. */
#define VERSAL_USB2_WRAPPER_MMIO_SIZE 0x100U

#define MMU_REGION_DWC3_USB2_WRAPPER_AMD(node_id, attrs)                              \
	IF_ENABLED(DT_NODE_HAS_PROP(node_id, amd_usb2_wrapper_base),                   \
		   (MMU_REGION_FLAT_ENTRY(DT_NODE_FULL_NAME(node_id) "_usb2_wrapper_amd", \
					   DT_PROP(node_id, amd_usb2_wrapper_base),     \
					   VERSAL_USB2_WRAPPER_MMIO_SIZE, attrs)))

#define MMU_REGION_DWC3_USB2_WRAPPER_XLNX(node_id, attrs)                             \
	IF_ENABLED(DT_NODE_HAS_PROP(node_id, xlnx_usb2_wrapper_base),                  \
		   (MMU_REGION_FLAT_ENTRY(DT_NODE_FULL_NAME(node_id) "_usb2_wrapper_xlnx",\
					   DT_PROP(node_id, xlnx_usb2_wrapper_base),    \
					   VERSAL_USB2_WRAPPER_MMIO_SIZE, attrs)))

#define MMU_REGION_DWC3_USB2_WRAPPER_FOREACH(attr)                                  \
	DT_FOREACH_STATUS_OKAY_VARGS(snps_dwc3, MMU_REGION_DWC3_USB2_WRAPPER_AMD, attr) \
	DT_FOREACH_STATUS_OKAY_VARGS(snps_dwc3, MMU_REGION_DWC3_USB2_WRAPPER_XLNX, attr)

static const struct arm_mmu_region mmu_regions[] = {
	/* DWC3 xHCI USB host (MMIO not covered by SRAM) */
	MMU_REGION_DT_COMPAT_FOREACH_FLAT_ENTRY(snps_dwc3, DEVICE_ATTR)

	/* Versal USB2_REGS wrapper when amd,usb2-wrapper-base is in DT */
	MMU_REGION_DWC3_USB2_WRAPPER_FOREACH(DEVICE_ATTR)
};

const struct arm_mmu_config mmu_config = {
	.num_regions = ARRAY_SIZE(mmu_regions),
	.mmu_regions = mmu_regions,
};

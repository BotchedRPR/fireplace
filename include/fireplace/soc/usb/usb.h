/*
 *   Copyright (c) 2025 Igor Belwon <igor.belwon@mentallysanemainliners.org>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, version 2.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef FIREPLACE_USB_H
#define FIREPLACE_USB_H

#include <unicorn/unicorn.h>

int usb_phy_init(struct uc_struct *uc_s);
void usb_phy_hook(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user_data);

int usb_init(struct uc_struct *uc_s);
void usb_hook(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user_data);

/* USB constants from lk3rd */

#define USB_PHY_BASE (0x10c00000)
#define USB_DWC_BASE (0x10e00000)

#define rGSBUSCFG0 (0xc100)
#define rGSBUSCFG1 (0xc104)
#define rGTXTHRCFG (0xc108)
#define rGRXTHRCFG (0xc10c)

#define rGCTL (0xc110)
#define rGPMSTS (0xc114)
#define rGSTS (0xc118)
#define rGUCTL1 (0xc11c)

#define rGSNPSID (0xc120)
#define rGGPIO (0xc124)
#define rGUID (0xc128)
#define rGUCTL (0xc12c)

#define rGBUSERRADDR_LO (0xc130)
#define rGBUSERRADDR_HI (0xc134)

//-----------------------------------------
// Global Port to USB Instance Mapping Registers
#define rGPRTBIMAP_LO (0xc138)
#define rGPRTBIMAP_HI (0xc13c)
#define rGDBGLSPMUX_DEV (0xc170)
#define rGPRTBIMAP_HS_LO (0xc180)
#define rGPRTBIMAP_HS_HI (0xc184)
#define rGPRTBIMAP_FS_LO (0xc188)
#define rGPRTBIMAP_FS_HI (0xc18c)

//-----------------------------------------
// Global Hardware Parameter Registers
#define rGHWPARAMS0 (0xc140)
#define rGHWPARAMS1 (0xc144)
#define rGHWPARAMS2 (0xc148)
#define rGHWPARAMS3 (0xc14c)
#define rGHWPARAMS4 (0xc150)
#define rGHWPARAMS5 (0xc154)
#define rGHWPARAMS6 (0xc158)
#define rGHWPARAMS7 (0xc15c)
#define rGHWPARAMS8 (0xc600)

//-----------------------------------------
// Global Debug Registers
#define rGDBGFIFOSPACE (0xc160)
#define rGDBGLTSSM (0xc164)
#define rGDBGLNMCC (0xc168)
#define rGDBGBMU (0xc16c)

#define rGDBGLSPMUX (0xc170)
#define rGDBGLSP (0xc174)
#define rGDBGEPINFO0 (0xc178)
#define rGDBGEPINFO1 (0xc17c)

#define rGERRINJCTL_1 (0xc194)
#define rGERRINJCTL_2 (0xc198)
#define rGUCTL2 (0xc19c)
#define rGPCIEL1EXTLAT (0xc1b4)

#define rDCFG (0xc700)
#define rDCTL (0xc704)
#define rDEVTEN (0xc708)
#define rDSTS (0xc70c)
#define rDGCMDPAR (0xc710)
#define rDGCMD (0xc714)
#define rDALEPENA (0xc720)

//-----------------------------------------
// Device Endpoint Registers (0 <= ep <= 31)
#define rDEPCMDPAR2(ep)		(0xc800 + 0x10*ep)
#define rDEPCMDPAR1(ep)		(0xc804 + 0x10*ep)
#define rDEPCMDPAR0(ep)		(0xc808 + 0x10*ep)
#define rDEPCMD(ep)		(0xc80c + 0x10*ep)

//-----------------------------------------
// Global Frame Length Adjustment Register
#define	rGFLADJ			(0xC630)

#define EXYNOS_USBCON_CTRL_VER (0x00)

#define EXYNOS_USBCON_LINK_CTRL (0x04)
#define LINKCTRL_PIPE3_FORCE_RX_ELEC_IDLE (0x1 << 18)
#define LINKCTRL_PIPE3_FORCE_PHY_STATUS (0x1 << 17)
#define LINKCTRL_PIPE3_FORCE_EN (0x1 << 16)
#define LINKCTRL_DIS_QACT_BUSPEND (0x1 << 13)
#define LINKCTRL_DIS_QACT_LINKGATE (0x1 << 12)
#define LINKCTRL_DIS_QACT_ID0 (0x1 << 11)
#define LINKCTRL_DIS_QACT_VBUS_VALID (0x1 << 10)
#define LINKCTRL_DIS_QACT_BVALID (0x1 << 9)
#define LINKCTRL_FORCE_QACT (0x1 << 8)
#define LINKCTRL_BUS_FILTER_BYPASS(_x) ((_x & 0xf) << 4)
#define LINKCTRL_BUS_FILTER_BYPASS_MASK (0xf << 4)
#define LINKCTRL_HOST_SYSTEM_ERR (0x1 << 2)
#define LINKCTRL_LINK_PME (0x1 << 1)
#define LINKCTRL_PME_GENERATION (0x1 << 0)

#define EXYNOS_USBCON_LINK_PORT (0x08)
#define LINKPORT_HOST_NUM_U3(_x) ((_x & 0xf) << 16)
#define LINKPORT_HOST_NUM_U2(_x) ((_x & 0xf) << 12)
#define LINKPORT_HOST_U3_PORT_DISABLE (0x1 << 9)
#define LINKPORT_HOST_U2_PORT_DISABLE (0x1 << 8)
#define LINKPORT_HOST_PORT_POWER_CON_PRESENT (0x1 << 6)
#define LINKPORT_HUB_PORT_SET_OCD_U3 (0x1 << 5)
#define LINKPORT_HUB_PORT_SET_OCD_U2 (0x1 << 4)
#define LINKPORT_HUB_PORT_SEL_OCD_U3 (0x1 << 3)
#define LINKPORT_HUB_PORT_SEL_OCD_U2 (0x1 << 2)
#define LINKPORT_HUB_PERM_ATTACH_U3 (0x1 << 1)
#define LINKPORT_HUB_PERM_ATTACH_U2 (0x1 << 0)

#define EXYNOS_USBCON_LINK_DEBUG_L (0x0C)
#define EXYNOS_USBCON_LINK_DEBUG_H (0x10)

#define EXYNOS_USBCON_LTSTATE_HIS (0x14)
#define LTSTATE_LINKTRN_DONE (0x1 << 31)
#define LTSTATE_HIS4(_x) ((_x & 0xf) << 16)
#define LTSTATE_HIS3(_x) ((_x & 0xf) << 12)
#define LTSTATE_HIS2(_x) ((_x & 0xf) << 8)
#define LTSTATE_HIS1(_x) ((_x & 0xf) << 4)
#define LTSTATE_HIS0(_x) ((_x & 0xf) << 0)

#define EXYNOS_USBCON_CLKRST (0x20)
#define CLKRST_USBAUDIO_CLK_GATE_EN (0x1 << 9)
#define CLKRST_USBAUDIO_CLK_SEL (0x1 << 8)
#define CLKRST_LINK_PCLK_SEL (0x1 << 7)
#define CLKRST_PHYCLOCKSEL (0x1 << 6)
#define CLKRST_PHY30_SW_RST (0x1 << 3)
#define CLKRST_PHY30_RST_SEL (0x1 << 2)
#define CLKRST_PHY20_SW_RST (0x1 << 13)
#define CLKRST_PHY20_RST_SEL (0x1 << 12)
#define CLKRST_PHY_SW_RST (0x1 << 3)
#define CLKRST_PHY_RST_SEL (0x1 << 2)
#define CLKRST_REFCLK_SEL (0x1 << 4)
#define CLKRST_PORT_RST (0x1 << 1)
#define CLKRST_LINK_SW_RST (0x1 << 0)

#define EXYNOS_USBCON_PWR (0x24)

#define PWR_PIPE3_POWERDONW (0xf << 4)
#define PWR_FORCE_POWERDOWN_EN (0x1 << 3)
#define RSVD1 (0x7 << 0)
#define PWR_FORCE_POWERDONW (0x1 << 2)
#define PWR_TEST_POWERDOWN_SSP (0x1 << 1)
#define PWR_TEST_POWERDOWN_HSP (0x1 << 0)

#define EXYNOS_USBCON_DUALPHYSEL (0x28)
#define DUALPHYSEL_PHYSEL_CTRL (0x1 << 0)
#define DUALPHYSEL_PHYSEL_SSPHY (0x1 << 1)
#define DUALPHYSEL_PHYSEL_PIPECLK (0x1 << 4)
#define DUALPHYSEL_PHYSEL_PIPERST (0x1 << 8)

#define EXYNOS_USBCON_SSP_PLL (0x30)
#define SSP_PLL_MPLL_MULTIPLIER_MASK (0x7F << 24)
#define SSP_PLL_MPLL_MULTIPLIER(_x) ((_x & 0x7f) << 24)
#define SSP_PLL_SSC_REF_CLK_SEL_MASK (0x1ff << 12)
#define SSP_PLL_SSC_REF_CLK_SEL(_x) ((_x & 0x1ff) << 12)
#define SSP_PLL_SSC_EN (0x1 << 11)
#define SSP_PLL_SSC_RANGE_MASK (0x7 << 8)
#define SSP_PLL_SSC_RANGE(_x) ((_x & 0x7) << 8)
#define SSP_PLL_REF_SSP_EN (0x1 << 7)
#define SSP_PLL_REF_CLKDIV2 (0x1 << 6)
#define SSP_PLL_FSEL_MASK (0x3f << 0)
#define SSP_PLL_FSEL(_x) ((_x & 0x3f) << 0)

#define EXYNOS_USBCON_SSP_PARACON0 (0x34)
#define SSP_PARACON0_TX0_TERM_OFFSET_MASK (0x1f << 25)
#define SSP_PARACON0_TX0_TERM_OFFSET(_x) ((_x & 0x1f) << 25)
#define SSP_PARACON0_PCS_TX_SWING_FULL_MASK (0x7f << 16)
#define SSP_PARACON0_PCS_TX_SWING_FULL(_x) ((_x & 0x7f) << 16)
#define SSP_PARACON0_PCS_TX_DEEMPH_6DB_MASK (0x3f << 8)
#define SSP_PARACON0_PCS_TX_DEEMPH_6DB(_x) ((_x & 0x3f) << 8)
#define SSP_PARACON0_PCS_TX_DEEMPH_3P5DB_MASK (0x3f << 0)
#define SSP_PARACON0_PCS_TX_DEEMPH_3P5DB(_x) ((_x & 0x3f) << 0)

#define EXYNOS_USBCON_SSP_PARACON1 (0x38)
#define SSP_PARACON1_TX_VBOOST_LVL_SSTX_MASK (0x7 << 28)
#define SSP_PARACON1_TX_VBOOST_LVL_SSTX(_x) ((_x & 0x7) << 28)
#define SSP_PARACON1_TX_VBOOST_LVL_MASK (0x7 << 24)
#define SSP_PARACON1_TX_VBOOST_LVL(_x) ((_x & 0x7) << 24)
#define SSP_PARACON1_LOS_LEVEL_MASK (0x1f << 16)
#define SSP_PARACON1_LOS_LEVEL(_x) ((_x & 0x1f) << 16)
#define SSP_PARACON1_LOS_BIAS_MASK (0x7 << 12)
#define SSP_PARACON1_LOS_BIAS(_x) ((_x & 0x7) << 12)
#define SSP_PARACON1_PCS_RX_LOS_MASK_VAL_MASK (0x3ff << 0)
#define SSP_PARACON1_PCS_RX_LOS_MASK_VAL(_x) ((_x & 0x3ff) << 0)

#define EXYNOS_USBCON_SSP_TEST (0x3C)
#define SSP_TEST_TX_EYE_HEIGHT_CNTL_EN_MASK (0x1 << 28)
#define SSP_TEST_TX_EYE_HEIGHT_CNTL_EN(_x) ((_x & 0x1) << 28)
#define SSP_TEST_PIPE_TX_DEEMPH_UPDATE_DELAY_MASK (0xf << 24)
#define SSP_TEST_PIPE_TX_DEEMPH_UPDATE_DELAY(_x) ((_x & 0xf) << 24)
#define SSP_TEST_PCS_TX_SWING_FULL_SSTX_MASK (0x7f << 16)
#define SSP_TEST_PCS_TX_SWING_FULL_SSTX(_x) ((_x & 0x7f) << 16)
#define SSP_TEST_RTUNE_ACK (0x1 << 3)
#define SSP_TEST_RTUNE_REQ (0x1 << 2)
#define SSP_TEST_LANE0_TX2RX_LOOPBK (0x1 << 1)
#define SSP_TEST_LOOPBACKENB (0x1 << 0)

#define EXYNOS_USBCON_SSP_CRCTL0 (0x40)
#define SSP_CCTRL0_CR_DATA_IN_MASK (0xffffU << 16)
#define SSP_CCTRL0_CR_DATA_IN(_x) ((_x & 0xffffU) << 16)
#define SSP_CRCTRL0_CR_WRITE (0x1 << 3)
#define SSP_CRCTRL0_CR_READ (0x1 << 2)
#define SSP_CRCTRL0_CR_CAP_DATA (0x1 << 1)
#define SSP_CRCTRL0_CR_CAP_ADDR (0x1 << 0)

#define EXYNOS_USBCON_SSP_CRCTL1 (0x44)
#define SSP_CRCTL1_CR_DATA_OUT_MASK (0xffffU << 16)
#define SSP_CRCTL1_CR_DATA_OUT(_x) ((_x & 0xffffU) << 16)
#define SSP_CRCTL1_CR_ACK (0x1 << 0)

#define EXYNOS_USBCON_COMBO_PMA_CTRL (0x48)
#define PMA_REF_SOC_PLL_SSC (0x1 << 16)
#define PMA_ROPLL_REF_REQ_MASK (0x3 << 12)
#define PMA_ROPLL_REF_REQ_SET(_x) ((_x & 0x3) << 12)
#define PMA_ROPLL_REF_REQ_GET(_x) ((_x & (0x3 << 12)) >> 12)
#define PMA_PLL_REF_REQ_MASK (0x3 << 10)
#define PMA_PLL_REF_REQ_SET(_x) ((_x & 0x3) << 10)
#define PMA_PLL_REF_REQ_GET(_x) ((_x & (0x3 << 10)) >> 10)
#define PMA_REF_FREQ_MASK (0x3 << 8)
#define PMA_REF_FREQ_SET(_x) ((_x & 0x3) << 8)
#define PMA_REF_FREQ_GET(_x) ((_x & (0x3 << 8)) >> 8)
#define PMA_LOW_PWRN (0x1 << 4)
#define PMA_TRSV_SW_RST (0x1 << 3)
#define PMA_CMN_SW_RST (0x1 << 2)
#define PMA_INIT_SW_RST (0x1 << 1)
#define PMA_APB_SW_RST (0x1 << 0)

#define EXYNOS_USBCON_UTMI (0x50)
#define UTMI_OPMODE_CTRL_EN (0x1 << 8)
#define UTMI_FORCE_OPMODE_MASK (0x3 << 6)
#define UTMI_FORCE_OPMODE_SET(_x) ((_x & 0x3) << 6)
#define UTMI_FORCE_VBUSVALID (0x1 << 5)
#define UTMI_FORCE_BVALID (0x1 << 4)
#define UTMI_DP_PULLDOWN (0x1 << 3)
#define UTMI_DM_PULLDOWN (0x1 << 2)
#define UTMI_FORCE_SUSPEND (0x1 << 1)
#define UTMI_FORCE_SLEEP (0x1 << 0)

#define EXYNOS_USBCON_HSP (0x54)
#define HSP_AUTORSM_ENB (0x1 << 29)
#define HSP_RETENABLE_EN (0x1 << 28)
#define HSP_FSLS_SPEED_SEL (0x1 << 25)
#define HSP_FSV_OUT_EN (0x1 << 24)
#define HSP_HS_XCVR_EXT_CTL (0x1 << 22)
#define HSP_HS_RXDAT (0x1 << 21)
#define HSP_HS_SQUELCH (0x1 << 20)
#define HSP_FSVMINUS (0x1 << 17)
#define HSP_FSVPLUS (0x1 << 16)
#define HSP_FSVPLUS_GET(_x) ((_x & (0x1 << 16)) >> 16)
#define HSP_VBUSVLDEXTSEL (0x1 << 13)
#define HSP_VBUSVLDEXT (0x1 << 12)
#define HSP_EN_UTMISUSPEND (0x1 << 9)
#define HSP_COMMONONN (0x1 << 8)
#define HSP_VATESTENB (0x1 << 6)
#define HSP_CHGDET (0x1 << 5)
#define HSP_CHGDET_GET(_x) ((_x & (0x1 << 5)) >> 5)
#define HSP_VDATSRCENB (0x1 << 4)
#define HSP_VDATDETENB (0x1 << 3)
#define HSP_CHRGSEL (0x1 << 2)
#define HSP_ACAENB (0x1 << 1)
#define HSP_DCDENB (0x1 << 0)

#define EXYNOS_USBCON_HSP_TUNE (0x58)
#define HSP_TUNE_TXVREF_MASK ((unsigned)0xf << 28)
#define HSP_TUNE_TXVREF_SET(_x) ((unsigned)(_x & 0xf) << 28)
#define HSP_TUNE_TXVREF_GET(_x) ((_x & (0xfU << 28)) >> 28)
#define HSP_TUNE_TXRISE_MASK (0x3 << 24)
#define HSP_TUNE_TXRISE_SET(_x) ((_x & 0x3) << 24)
#define HSP_TUNE_TXRISE_GET(_x) ((_x & (0x3 << 24)) >> 24)
#define HSP_TUNE_TXRES_MASK (0x3 << 21)
#define HSP_TUNE_TXRES_SET(_x) ((_x & 0x3) << 21)
#define HSP_TUNE_TXRES_GET(_x) ((_x & (0x3 << 21)) >> 21)
#define HSP_TUNE_TXPREEMPA_PLUS (0x1 << 20)
#define HSP_TUNE_TXPREEMPA_PLUS_GET(_x) ((_x & (0x1 << 20)) >> 20)
#define HSP_TUNE_TXPREEMPA_MASK (0x3 << 18)
#define HSP_TUNE_TXPREEMPA_SET(_x) ((_x & 0x3) << 18)
#define HSP_TUNE_TXPREEMPA_GET(_x) ((_x & (0x3 << 18)) >> 18)
#define HSP_TUNE_HSXV_MASK (0x3 << 16)
#define HSP_TUNE_HSXV_SET(_x) ((_x & 0x3) << 16)
#define HSP_TUNE_HSXV_GET(_x) ((_x & (0x3 << 16)) >> 16)
#define HSP_TUNE_TXFSLS_MASK (0xf << 12)
#define HSP_TUNE_TXFSLS_SET(_x) ((_x & 0xf) << 12)
#define HSP_TUNE_TXFSLS_GET(_x) ((_x & (0xf << 12)) >> 12)
#define HSP_TUNE_SQRX_MASK (0x7 << 8)
#define HSP_TUNE_SQRX_SET(_x) ((_x & 0x7) << 8)
#define HSP_TUNE_SQRX_GET(_x) ((_x & (0x7 << 8)) >> 8)
#define HSP_TUNE_OTG_MASK (0x7 << 4)
#define HSP_TUNE_OTG_SET(_x) ((_x & 0x7) << 4)
#define HSP_TUNE_OTG_GET(_x) ((_x & (0x7 << 4)) >> 4)
#define HSP_TUNE_COMPDIS_MASK (0x7 << 0)
#define HSP_TUNE_COMPDIS_SET(_x) ((_x & 0x7) << 0)
#define HSP_TUNE_COMPDIS_GET(_x) ((_x & (0x7 << 0)) >> 0)

#define EXYNOS_USBCON_HSP_TEST (0x5c)
#define HSP_TEST_HS_RXDAT (0x1 << 26)
#define HSP_TEST_HS_SQUELCH (0x1 << 25)
#define HSP_TEST_SIDDQ (0x1 << 24)
#define HSP_TEST_LINESTATE_MASK (0x3 << 20)
#define HSP_TEST_LINESTATE_SET(_x) ((_x & 0x3) << 20)
#define HSP_TEST_LINESTATE_GET(_x) ((_x & (0x3 << 20)) >> 20)
#define HSP_TEST_DATA_OUT_MASK (0xf << 16)
#define HSP_TEST_DATA_OUT_SET(_x) ((_x & 0xf) << 16)
#define HSP_TEST_DATA_OUT_GET(_x) ((_x & (0xf << 16)) >> 16)
#define HSP_TEST_CLK (0x1 << 13)
#define HSP_TEST_DATA_OUT_SEL (0x1 << 12)
#define HSP_TEST_DATA_ADDR_MASK (0xf << 8)
#define HSP_TEST_DATA_ADDR_SET(_x) ((_x & 0xf) << 8)
#define HSP_TEST_DATA_ADDR_GET(_x) ((_x & (0xf << 8)) >> 8)
#define HSP_TEST_DATA_IN_MASK (0xff << 0)
#define HSP_TEST_DATA_IN_SET(_x) ((_x & 0xff) << 0)
#define HSP_TEST_DATA_IN_GET(_x) ((_x & (0xff << 0)) >> 0)

#define EXYNOS_USBCON_HSP_PLL_TUNE (0x60)
#define HSP_PLL_BTUNE (0x1 << 8)
#define HSP_PLL_ITUNE_MASK (0x3 << 4)
#define HSP_PLL_ITUNE_IN_SET(_x) ((_x & 0x3) << 4)
#define HSP_PLL_ITUNE_IN_GET(_x) ((_x & (0x3 << 4)) >> 4)
#define HSP_PLL_PTUNE_MASK (0xf << 0)
#define HSP_PLL_PTUNE_IN_SET(_x) ((_x & 0xf) << 0)
#define HSP_PLL_PTUNE_IN_GET(_x) ((_x & (0xf << 0)) >> 0)

/* Remote Wake-up Advisro (ReWA) */
#define EXYNOS_USBCON_REWA_ENABLE (0x100)
#define REWA_ENABLE_SS_REWA_EN (0x1 << 8)
#define REWA_ENABLE_HS_REWA_EN (0x1 << 0)

#define EXYNOS_USBCON_HSREWA_INTR (0x104)
#define HSREWA_INTR_WAKEUP_REQ_MASK (0x1 << 12)
#define HSREWA_INTR_TIMEOUT_MASK (0x1 << 8)
#define HSREWA_INTR_EVT_MASK (0x1 << 4)
#define HSREWA_INTR_WAKEUP_MASK (0x1 << 0)

#define EXYNOS_USBCON_HSREWA_CTRL (0x108)
#define HSREWA_CTRL_DIG_BYPASS_CON_EN (0x1 << 28)
#define HSREWA_CTRL_DPDM_MON_SEL (0x1 << 24)
#define HSREWA_CTRL_HS_LINK_READY (0x1 << 20)
#define HSREWA_CTRL_HS_SYS_VALID (0x1 << 16)
#define HSREWA_CTRL_HS_REWA_ERR (0x1 << 4)
#define HSREWA_CTRL_HS_REWA_DONE (0x1 << 0)

#define EXYNOS_USBCON_HSREWA_REFTO (0x10C)

#define EXYNOS_USBCON_HSREWA_HSTK (0x110)

#define EXYNOS_USBCON_HSREWA_CNT (0x114)

#define EXYNOS_USBCON_HSREWA_INT1_EVT (0x118)
#define HSREWA_CTRL_HS_EVT_ERR_SUS (0x1 << 18)
#define HSREWA_CTRL_HS_EVT_ERR_DEV_K (0x1 << 17)
#define HSREWA_CTRL_HS_EVT_DISCON (0x1 << 16)
#define HSREWA_CTRL_HS_EVT_BYPASS_DIS (0x1 << 2)
#define HSREWA_CTRL_HS_EVT_RET_DIS (0x1 << 1)
#define HSREWA_CTRL_HS_EVT_RET_EN (0x1 << 0)

#define EXYNOS_USBCON_HSREWA_INT1_MASK (0x11C)

#endif
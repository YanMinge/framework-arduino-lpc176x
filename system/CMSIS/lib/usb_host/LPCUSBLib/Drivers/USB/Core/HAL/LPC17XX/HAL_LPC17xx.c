/*
 * @brief HAL USB functions for the LPC17xx microcontrollers
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#if defined(__LPC175X_6X__) || defined(__LPC177X_8X__) || defined(__LPC407X_8X__)

#include "../HAL.h"
#include "../../USBTask.h"
#include "lpc17xx_pinsel.h"

#define SYSCTL_PLL_ENABLE   (1 << 0)/*!< PLL enable flag */

void HAL_USBInit(uint8_t corenum)
{
	PINSEL_CFG_Type PinCfg;

	// Initialize USB pin connect
	/* P0.29 D1+, P0.30 D1- */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 29;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 30;
	PINSEL_ConfigPin(&PinCfg);

#if defined(USB_CAN_BE_HOST)
	/* USB_Power switch */
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 19;
	PinCfg.Portnum = 1;
	PINSEL_ConfigPin(&PinCfg);
#endif
	
#if defined(USB_CAN_BE_DEVICE)
    /* USB_SoftConnect */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 9;
	PinCfg.Portnum = 2;
	PINSEL_ConfigPin(&PinCfg);
#endif
	
	LPC_SC->PCONP |= (1UL << 31);					/* USB PCLK -> enable USB Per.*/

#if defined(USB_CAN_BE_DEVICE)
	LPC_USB->USBClkCtrl = 0x12;					    /* Dev, PortSel, AHB clock enable */
	while ((LPC_USB->USBClkSt & 0x12) != 0x12) ;

	HAL_Reset(corenum);
#endif
}

void HAL_USBDeInit(uint8_t corenum, uint8_t mode)
{
	PINSEL_CFG_Type PinCfg;

	NVIC_DisableIRQ(USB_IRQn);										   /* disable USB interrupt */
	LPC_SC->PCONP &= (~(1UL << 31));								   /* disable USB Per.      */

	/* P0.29 D+, P0.30 D- reset to GPIO function */
    PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 29;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 30;
	PINSEL_ConfigPin(&PinCfg);

	/* Disable PLL1 to save power */
    uint32_t temp;
	temp = LPC_SC->PLL1CON;
	temp &= ~SYSCTL_PLL_ENABLE;
	LPC_SC->PLL1CON = temp;
    LPC_SC->PLL1FEED  = 0xAA;
    LPC_SC->PLL1FEED  = 0x55;
}

void HAL_EnableUSBInterrupt(uint8_t corenum)
{
	NVIC_EnableIRQ(USB_IRQn);					/* enable USB interrupt */
}

void HAL_DisableUSBInterrupt(uint8_t corenum)
{
	NVIC_DisableIRQ(USB_IRQn);					/* enable USB interrupt */
}

void HAL_USBConnect(uint8_t corenum, uint32_t con)
{
	if (USB_CurrentMode[corenum] == USB_MODE_Device) {
#if defined(USB_CAN_BE_DEVICE)
		HAL17XX_USBConnect(con);
#endif
	}
}

// TODO moving stuff to approriate places
extern void DcdIrqHandler (uint8_t DeviceID);

void USB_IRQHandler(void)
{
	if (USB_CurrentMode[0] == USB_MODE_Host) {
		#if defined(USB_CAN_BE_HOST)
		HcdIrqHandler(0);
		#endif
	}

	if (USB_CurrentMode[0] == USB_MODE_Device) {
		#if defined(USB_CAN_BE_DEVICE)
		DcdIrqHandler(0);
		#endif
	}
}

#endif /*__LPC17XX__ || __LPC40XX__*/

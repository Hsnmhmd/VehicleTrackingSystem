/******************************************************************************
 * File Name: gsm_hw.h
 *
 * Description: Header file for GSM integration.
 *
 * Author: AVELABS_D
 *
 * Date : Aug 27 2023
 *******************************************************************************/

#ifndef HAL_GSM_HW_H_
#define HAL_GSM_HW_H_


/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define GSMUART_Base     UART2_BASE


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void GSMInit(void);
void GSMSend(uint8_t *CMD_Data,uint32_t DataSize);
void GSMReceiveResponse(uint8_t *Response,uint32_t ResponseSize);
void GSMSetReceptionCallBack(void (*Callback)(void));
void GSMSetTransmissionCallBack(void (*Callback)(void));

#endif /* HAL_GSM_HW_H_ */

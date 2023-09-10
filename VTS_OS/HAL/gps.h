/******************************************************************************
 * File Name  :   gps.h
 *
 * Description: Header file for the gps swc
 *
 * Author     : Avelabs_D
 *
 * Date       : 27 Aug, 2023
 *******************************************************************************/

#ifndef HAL_GPS_H_
#define HAL_GPS_H_

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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define GPSUART_Base     UART1_BASE
typedef enum
{
    UTURN,
    STRAIGHT_LINE,
    CURVE,
}UTurn_Status;


/*******************************************************************************
 *                              Threshold Angle Declaration                     *
 *******************************************************************************/
#define CURVE_ANGLE  4
#define UTRURN_ANGLE 30
#define Low_Speed 20
#define High_Speed 40

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*                      Function to initialize GPS module                      */
void GPSInit(void);

/*       Function that enables DMA channel to start a new data transfer        */
void GPSGetRawData(uint8_t *RawData,uint32_t RawDataSize);
/*                      Function to set callback Function                      */
void GPSSetReceptionCallBack(void (*Callback)(void));
/*             The core function which takes the raw data and parse it          */
void GPSParseRawData(char Received_Data[], float *Time, char *Longitude, char *Latitude, float *Speed, float *currentCOG,char *State);
/*             The function That Detects the type of movement                   */
int GPSDetectUTurn(const float currentCOG,const float speed);

#endif /* HAL_GPS_H_ */

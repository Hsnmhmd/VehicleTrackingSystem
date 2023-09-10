/******************************************************************************
 * File Name: SIM800.h
 *
 * Description: Header file for SIM800 module integration.
 *
 * Author: AVELABS_D
 *
 * Date : Aug 27 2023
 *******************************************************************************/


#ifndef SRC_Sim800_H_
#define SRC_Sim800_H_

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include <HAL/gsm_hw.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define Sim800BufSize  200

typedef enum{
    Gsmok=0,
    GsmError,
    GsmReady
}GsmStatus;


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void sim800recieve(void);
void Sim800Init(void);
void Sim800PrepareLink(char *RQSTLink,char *Lon, char *Lat);
uint32_t Sim800SetNetConnectivity(void);
uint32_t Sim800HttpRequest(char *Lon, char *Lat);
#endif /* SRC_Sim800_H_ */

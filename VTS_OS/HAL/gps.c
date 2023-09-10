/************************************************************************************************
 * File Name  :   gps.c
 *
 * Description: Source file for the gps swc
 *
 * Author     : Avelabs_D
 *
 * Date       : 27 Aug, 2023
 ***********************************************************************************************/

#include "gps.h"

/************************************************************************************************
 *                               Global_Variables Declaration                                   *
 ************************************************************************************************/

extern uint32_t uDMAControlTable[256];
extern void (* UART1RX_DMA_CH8_Ptr )(void);


/*******************************************************************************
 *                        Private Functions Prototypes                         *
 *******************************************************************************/
static void GPSInitPortPinsAndClock(void);
static void GPSUARTInit(void);
static void GPSDMAInit(void);


/***********************************************************************************************
 * Function Name      : GPSInit
 * Description        : Function to initialize GPS module
 * INPUTS             : void
 * RETURNS            : void
 ***********************************************************************************************/
void GPSInit(void)
{
/*                  Initialize pins connected to GPS module                    */
    GPSInitPortPinsAndClock();
/*              Initialize UART channel connected to GPS module                */
    GPSUARTInit();
/*           Initialize DMA channel to transfer received GPS data              */
    GPSDMAInit();
}


/***********************************************************************************************
 * Function Name      : GPSSetReceptionCallBack
 * Description        : Function to set callback Function
 * INPUTS             : Pointer to function
 * RETURNS            : void
 ***********************************************************************************************/
void GPSSetReceptionCallBack(void (*Callback)(void)){
    UART1RX_DMA_CH8_Ptr=Callback;
}


/***********************************************************************************************
 * Function Name      : GPSDMAInit
 * Description        : Initialize DMA and assign channel
 * INPUTS             : void
 * RETURNS            : void
 ***********************************************************************************************/
void GPSDMAInit(void){
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    uDMAControlBaseSet(uDMAControlTable);
    uDMAChannelAttributeEnable(UDMA_SEC_CHANNEL_UART1RX, UDMA_ATTR_HIGH_PRIORITY);


    uDMAChannelControlSet(UDMA_SEC_CHANNEL_UART1RX |UDMA_PRI_SELECT, UDMA_DST_INC_8| UDMA_SRC_INC_NONE |UDMA_SIZE_8 |UDMA_ARB_4
                            );
    uDMAChannelAssign(UDMA_CH8_UART1RX);
    uDMAEnable();
}

/***********************************************************************************************
 * Function Name      : GPSUARTInit
 * Description        : Initialize UART channel connected to GPS module and enable DMA
 * INPUTS             : void
 * RETURNS            : void
 ***********************************************************************************************/
void GPSUARTInit(void){
    UARTConfigSetExpClk(GPSUART_Base, 16000000, 9600, UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE);
    UARTDMAEnable(GPSUART_Base, UART_DMA_RX);

    //ENable NVIC FOR UART1
    IntEnable(INT_UART1_TM4C123);
}


/***********************************************************************************************
 * Function Name      : GPSGetRawData
 * Description        : Function that enables DMA channel to start a new data transfer
 * INPUTS             : Pointer to buffer in which the GPS data will be transfered,
 *                      Buffer size
 * RETURNS            : void
 ***********************************************************************************************/
void GPSGetRawData(uint8_t *RawData,uint32_t RawDataSize){
    uDMAChannelTransferSet( UDMA_SEC_CHANNEL_UART1RX |UDMA_PRI_SELECT, UDMA_MODE_BASIC,
                           (void *)(uint32_t)&HWREG(UART1_BASE+UART_O_DR), (void *)RawData,RawDataSize);
    uDMAChannelEnable(UDMA_SEC_CHANNEL_UART1RX);

}


/***********************************************************************************************
 * Function Name      : GPSInitPortPinsAndClock
 * Description        : Configure GPS pins and enable peripheral clock
 * INPUTS             : void
 * RETURNS            : void
 ***********************************************************************************************/
void GPSInitPortPinsAndClock(void){
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Enable pin PB0 for UART1 U1RX
    //
    MAP_GPIOPinConfigure(GPIO_PB0_U1RX);
    MAP_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0);

    //
    // Enable pin PB1 for UART1 U1TX
    //
    MAP_GPIOPinConfigure(GPIO_PB1_U1TX);
    MAP_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_1);

}


/***********************************************************************************************
 * Function Name      : GPSParseRawData
 * Description        : Parse the incoming data from gps module
 * INPUTS             : Recieved Data Buffer, pointer to time variable,
 *                      pointer to Longitude variable, pointer to Speed variable,
 *                      pointer to current COG variable and pointer to state variable
 * RETURNS            : void
 ***********************************************************************************************/
void GPSParseRawData(char Received_Data[], float *Time, char *Longitude, char *Latitude, float *Speed, float *currentCOG,char *State)
{
/*                      Payload that contain Time,Location,Speed                                */
    char GPS_Payload[120];
/*                           counter to loop around the message                                 */
    uint8_t  message_index=0;
/*                   pointer used to point the message char by char                             */
    char *ptr = NULL;

    ptr = strstr((const char*)Received_Data, "GPRMC");
    if (*ptr == 'G')
    {
        while (1)
        {
            GPS_Payload[message_index] = *ptr;
            message_index++;
            ptr++;
            if (*ptr == '\n')
            {
                GPS_Payload[message_index] = '\0';
                break;
            }
        }

        // Parse the GPS Payload manually
        char *token = strtok(GPS_Payload, ",");
        int tokenIndex = 0;
        while (token != NULL)
        {
            if (tokenIndex == 1)
            {
                *Time = atof(token);
            }
        else if(tokenIndex == 2)
            {
                 *State = *token;
        }
            else if (tokenIndex == 3)
            {
                strcpy(Latitude, token);
            }
            else if (tokenIndex == 5)
            {
                strcpy(Longitude, token);
            }
            else if (tokenIndex == 7)
            {
                *Speed = atof(token);
            }
            else if (tokenIndex == 8)
            {
                *currentCOG = atof(token);
            }
            token = strtok(NULL, ",");
            tokenIndex++;
        }

        *Speed =(*Speed)*1.852;
    }

}

 /**********************************************************************************************
 * Function Name: detectUTurn
 * Description  : Function to determine if there is a U-turn based on COG(Course Over Ground)
 *                readings that comes from the GPS Module output Data.
 * INPUTS       : const float currentCOG
 *
 * RETURNS      : UTurn_Status  Status (vehicle in U_Turn or Not)
 ***********************************************************************************************/
int GPSDetectUTurn(const float currentCOG,const float speed)
{
    UTurn_Status Status = STRAIGHT_LINE;                     /* a variable to return the status in */

    static float previousCOG = 0.0;                     /* Static variable to store previous COG */
    if(previousCOG==0){
        previousCOG=currentCOG;
        return Status;
    }
    float angleChange = fabs(currentCOG - previousCOG); /* Get the absolute value of change in direction angle */
    previousCOG = currentCOG;                           /* Update previousCOG for the next iteration */

    /* If the angle change is greater than a threshold, it indicates a  specific state of U-turn */
    if ( ( speed <= High_Speed) && (angleChange > UTRURN_ANGLE))
    {
        Status =  UTURN; //DOING A UTURN OR PARKING
    }
   else if ((speed >= Low_Speed ) && ((angleChange > CURVE_ANGLE) && (angleChange < UTRURN_ANGLE )))
    {
        Status =  CURVE;  //CURVING ON A LARGE RADUIS
    }
    else
    {
        Status =  STRAIGHT_LINE;
    }
    return Status;
}

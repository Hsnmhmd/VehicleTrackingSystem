/******************************************************************************
 * File Name: gsm_hw.c
 *
 * Description: Source file for GSM integration.
 *
 * Author: AVELABS_D
 *
 * Date : Aug 27 2023
 *******************************************************************************/

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include <HAL/gsm_hw.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
extern void (* UART2RX_DMA_CH0_Ptr )(void);
extern void (* UART2TX_DMA_CH1_Ptr )(void);
extern uint32_t uDMAControlTable[256];

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
static void GSMInitPortPinsAndClock(void);
static void GSMUARTInit(void);
static void GSMDMAInit(void);

/***********************************************************************************************
 * Function Name      : GSMInit
 * Description        : Initializing GSM
 * INPUTS             : void
 * RETURNS            : void
 ***********************************************************************************************/
void GSMInit(void){
    GSMInitPortPinsAndClock();
    //Init UART
    GSMUARTInit();
    //Init DMA
    GSMDMAInit();
}


/***********************************************************************************************
 * Function Name      : GSMSetReceptionCallBack
 * Description        : Set the callback function for GSM reception using DMA.
 * INPUTS             : void (*Callback)(void)
 * RETURNS            : void
 ***********************************************************************************************/
void GSMSetReceptionCallBack(void (*Callback)(void)){
    // Assign the provided callback function to the pointer responsible for DMA reception.
    UART2RX_DMA_CH0_Ptr=Callback;
}

/***********************************************************************************************
 * Function Name      : GSMSetTransmissionCallBack
 * Description        : Set the callback function for GSM transmission using DMA.
 * INPUTS             : void (*Callback)(void)
 * RETURNS            : void
 ***********************************************************************************************/
void GSMSetTransmissionCallBack(void (*Callback)(void)){
    // Assign the provided callback function to the pointer responsible for DMA reception.
    UART2TX_DMA_CH1_Ptr=Callback;
}

/***********************************************************************************************
 * Function Name      : GSMReceiveResponse
 * Description        : Set the callback function for GSM transmission using DMA.
 * INPUTS             : uint8_t *Response, uint32_t ResponseSize
 * RETURNS            : void
 ***********************************************************************************************/
void GSMReceiveResponse(uint8_t *Response,uint32_t ResponseSize){
    // Configure DMA transfer settings for UART2 RX channel.
    uDMAChannelTransferSet( UDMA_SEC_CHANNEL_UART2RX_0 |UDMA_PRI_SELECT, UDMA_MODE_BASIC,
                           (void *)&HWREG(UART2_BASE+UART_O_DR), (void *)Response,ResponseSize);
    // Enable the DMA channel for UART2 RX.
    uDMAChannelEnable(UDMA_SEC_CHANNEL_UART2RX_0);
}

/***********************************************************************************************
 * Function Name      : GSMSend
 * Description        : Send data using DMA.
 * INPUTS             : uint8_t *CMD_Data, uint32_t DataSize
 * RETURNS            : void
 ***********************************************************************************************/
void GSMSend(uint8_t *CMD_Data,uint32_t DataSize){
    // Configure DMA transfer settings for UART2 TX channel.
    uDMAChannelTransferSet( UDMA_SEC_CHANNEL_UART2TX_1 |UDMA_PRI_SELECT, UDMA_MODE_BASIC,
                           (void *)CMD_Data, (void *)&HWREG(UART2_BASE+UART_O_DR),DataSize);

    // Enable the DMA channel for UART2 TX.
    uDMAChannelEnable(UDMA_SEC_CHANNEL_UART2TX_1);

}

/***********************************************************************************************
 * Function Name      : GSMDMAInit
 * Description        : Initialize UART for GSM communication.
 * INPUTS             : void
 * RETURNS            : void
 ***********************************************************************************************/
void GSMDMAInit(void){

    // Enable the uDMA peripheral.
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);

    // Set the base address for the uDMA control table.
    uDMAControlBaseSet(uDMAControlTable);

    // Configure DMA channel for transmission (UART2 TX).
    uDMAChannelAttributeEnable(UDMA_SEC_CHANNEL_UART2TX_1, UDMA_ATTR_HIGH_PRIORITY);
    // Configure the control settings for the transmission channel.
    uDMAChannelControlSet(UDMA_SEC_CHANNEL_UART2TX_1 |UDMA_PRI_SELECT, UDMA_DST_INC_NONE| UDMA_SRC_INC_8 |UDMA_SIZE_8 |UDMA_ARB_4);
    // Assign the transmission channel to UART2 TX.
    uDMAChannelAssign(UDMA_CH1_UART2TX);
    // Configure DMA channel for reception (UART2 RX).
    uDMAChannelAttributeEnable(UDMA_SEC_CHANNEL_UART2RX_0, UDMA_ATTR_HIGH_PRIORITY);
    // Configure the control settings for the reception channel.
    uDMAChannelControlSet(UDMA_SEC_CHANNEL_UART2RX_0 |UDMA_PRI_SELECT, UDMA_DST_INC_8| UDMA_SRC_INC_NONE |UDMA_SIZE_8 |UDMA_ARB_4);
    // Assign the reception channel to the relevant UART2 receive channel.
    uDMAChannelAssign(UDMA_CH0_UART2RX);

    /*Enable The DMA*/
    uDMAEnable();
}

/***********************************************************************************************
 * Function Name      : GSMUARTInit
 * Description        : Initialize UART settings for GSM communication.
 * INPUTS             : void
 * RETURNS            : void
 ***********************************************************************************************/
void GSMUARTInit(void){
    // Configure UART settings:
    UARTConfigSetExpClk(GSMUART_Base, 16000000, 9600, UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE);
    // Enable DMA for UART transmit and receive.
    UARTDMAEnable(GSMUART_Base, UART_DMA_TX|UART_DMA_RX);
    // Enable NVIC interrupt for UART2.
    IntEnable(INT_UART2_TM4C123);
}


/***********************************************************************************************
 * Function Name      : GSMInitPortPinsAndClock
 * Description        : Initialize port pins and clock settings for GSM communication.
 * INPUTS             : void
 * RETURNS            : void
 ***********************************************************************************************/
void GSMInitPortPinsAndClock(void){
    // Enable the UART2 peripheral.
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    // Enable the GPIO port D peripheral.
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    // Enable pin PD7 for UART2 U2TX
    // First open the lock and select the bits we want to modify in the GPIO commit register.
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;
    // Now modify the configuration of the pins that we unlocked.
    MAP_GPIOPinConfigure(GPIO_PD7_U2TX);
    MAP_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_7);
    // Enable pin PD6 for UART2 U2RX
    MAP_GPIOPinConfigure(GPIO_PD6_U2RX);
    MAP_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6);
}

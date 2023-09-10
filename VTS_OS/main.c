/*===========================================================================================
 * Filename   : main.c
 * Author     : AVELABS_D
 * Description: Main File for Vehicle_Tracking_System
 * Created on : Aug 27 2023
 *==========================================================================================*/

/*===========================================================================================
 * SUMMARY:
 * The goal of the system is to continuously determine the location of a vehicle using
 * a GPS module and transmit this location data to a web interface or a location visualization
 * tool using GSM Module, allowing for round-the-clock vehicle tracking.
 *==========================================================================================*/

/*===========================================================================================
 * Specification
 *
 * MicroController    : TIVA C
 * CPU Frequency      : 16 MHZ
 *
 * ********************
 * DRIVERS
 * ********************
 * MCAL               :
 * HAL                : GSM and GPS
 * APP                : FreeRTOS Based main.c && SIM800.c
 *==========================================================================================*/


/*******************************************************************************
 *                               Includes                                      *
 *******************************************************************************/
#include <HAL/gsm_hw.h>
#include "driverlib/udma.h"
#include "driverlib/uart.h"
#include "stdint.h"
#include "HAL/gps.h"
#include "SIM800.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "event_groups.h"
#include "FreeRTOSConfig.h"
#include "semphr.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define Trans_SIZE  500
#define  timeoutvalue       pdMS_TO_TICKS( 10000 )
#define  GPS_Priorities     (1)
#define  GPSRead_Priority   (2)
#define  GSM_Priorities     (3)

/*Synchronization Flags*/
#define GPS_RecFlag     (1<<0)
#define GPS_ParseFlag   (1<<1)
#define GPS_ValidFlag   (1<<2)
#define GSM_RateFlag     (1<<3)
#define GPS_ProcessFlag     (1<<4)
#define TimerFlag     (1<<5)
#define GSM_RecFlag     (1<<6)
#define GSM_ConFlag     (1<<7)
#define GPS_NewReadIssued     (1<<8)

/*******************************************************************************
 *                               Global_Variables Declaration                  *
 *******************************************************************************/
uint8_t RecieveBuffer[Trans_SIZE];
extern uint8_t buffer2[Sim800BufSize];

/*Variables to store TIME,SPEED, COURSE OVER GROUND*/
float Time, Speed, currentCOG;
UTurn_Status CMovementStatus=STRAIGHT_LINE;
UTurn_Status PMovementStatus=STRAIGHT_LINE;
char State;

/* Buffers to store the Langitude and Latitude */
char Longitude[12]="31.202", Latitude[12]="31";

/*Commands for GSMCheckConnection and GSMSendSequence */
extern uint8_t     InitHTTP[14];
extern uint8_t     EnableHTTPS[15];
extern uint8_t     SetCIDPAR[22];
extern uint8_t     HTTPRequest[136];
extern uint8_t     TERMINATEHTTP[14];
char     RQSTLink[200];
uint8_t* Commands[4]={SetCIDPAR,(uint8_t*)RQSTLink,HTTPRequest,TERMINATEHTTP};

/*Create Task Handles Create */
xTaskHandle GPSParseHand = NULL;
xTaskHandle GPSReadHand = NULL;
xTaskHandle GPSProcessDataHand = NULL;
xTaskHandle SetTimerRateHand = NULL;
xTaskHandle GSMCheckConnectionHand = NULL;
xTaskHandle GSMSendSequenceHand = NULL;

/* Declare a variable to hold the created event group. */
EventGroupHandle_t FlagsEventGroup;
/*Data Semaphore*/
xSemaphoreHandle DataSemaphore = NULL;
xSemaphoreHandle MovementSemaphore = NULL;
/*Timer Handler*/
TimerHandle_t GSMTimer;

/*******************************************************************************
 *                              Tasks Prototypes                           *
 *******************************************************************************/

/* parsing the incoming data from GPS module */
void GPSParse( void* pvParamter);
/* Reads GPS data From GPS Module */
void GPSRead( void* pvParamter);
/* Process the data After reading it from GPS Module */
void GPSProcessData( void* pvParamter);
/* SET The Rate in which we send to the Network based on the Movement Status */
void SetTimerRate( void* pvParamter);
/* Check GSM Connection before sending the Data */
void GSMCheckConnection(void* pvParamter);
/* Function to Send a Sequence of Commands To GSM Module */
void GSMSendSequence(void* pvParamter);


/*GSMTimer Callback*/
void GSMTimerCallback( TimerHandle_t GSMTimer )
{
    xEventGroupSetBits( FlagsEventGroup,  TimerFlag );
}
/* GPSSetFlag CallBack */
void GPSSetFlag(void)
{
    xEventGroupSetBitsFromISR( FlagsEventGroup,  GPS_RecFlag ,pdFALSE);
}
/* sim800recieve CallBack */
void sim800recieve(void)
{
    xEventGroupSetBitsFromISR( FlagsEventGroup,  GSM_RecFlag ,pdFALSE);
}

/*******************************************************************************
 *                               Main                                          *
 *******************************************************************************/
int main(void)
{
    //Create FreeRTOS Tasks
    xTaskCreate(GPSParse,"GPSParse",100,NULL,GPS_Priorities,&GPSParseHand);
    xTaskCreate(GPSRead,"GPSIssueRead",100,NULL,GPSRead_Priority,&GPSReadHand);
    xTaskCreate(GPSProcessData,"GPSProcess",100,NULL,GPS_Priorities,&GPSProcessDataHand);
    xTaskCreate(SetTimerRate,"SetTimerPeriod",100,NULL,GPS_Priorities,&SetTimerRateHand);
    xTaskCreate(GSMCheckConnection,"CheckConnection",100,NULL,GPS_Priorities,&GSMCheckConnectionHand);
    xTaskCreate(GSMSendSequence,"SendSequence",100,NULL,GSM_Priorities,&GSMSendSequenceHand);
    /*Create Semaphore for the Data and Movement Variables */
    vSemaphoreCreateBinary(DataSemaphore);
    vSemaphoreCreateBinary(MovementSemaphore);
    /* Attempt to create the event group. */
    FlagsEventGroup = xEventGroupCreate();
    GSMTimer = xTimerCreate("GSMTimer",pdMS_TO_TICKS( 30000 ),
    pdTRUE,/* The timers will auto-reload themselves when they expire. */
    ( void * ) 0,/* Each timer calls the same callback when it expires. */
    GSMTimerCallback );
    //Start The Timer
    xTimerStart( GSMTimer, 5000 );
    /*Init The GSM and GPS with Callbacks*/
    GPSInit();
    GPSSetReceptionCallBack(GPSSetFlag);
    GSMInit();
    GSMSetReceptionCallBack(sim800recieve);
    /* Set the Interrupts  to be less than the FreeRTOS ISRs priorities*/
    IntPrioritySet(INT_UART2, 0xE0);
    IntPrioritySet(INT_UART1, 0xE0);
    /* Initialize The GSM Connection*/
    Sim800SetNetConnectivity();
    /* Issue the first read operation*/
    GPSGetRawData(RecieveBuffer, Trans_SIZE);
    /*start Scheduler */
    vTaskStartScheduler();
    while(1){

    }
    return 0;
}
/***********************************************************************************************
 * Function Name      : GPSParse FreeRTOS Task
 * Description        : parsing the incoming data from gps module
 * INPUTS             : void* pvParamter
 * RETURNS            : void
 ***********************************************************************************************/
void GPSParse( void* pvParamter){
    EventBits_t uxBits;
    while(1){
//        if(xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED){
//            xTaskResumeAll();
//        }
        uxBits = xEventGroupWaitBits( FlagsEventGroup, GPS_RecFlag,  pdTRUE, pdTRUE, timeoutvalue );
        //Clear Flag on return
        if( ( uxBits & GPS_RecFlag ) == GPS_RecFlag )
        {
            //Ensure Atomic Access to the Parsed Data Variables
            if(xSemaphoreTake(DataSemaphore,portMAX_DELAY)){
            GPSParseRawData((char*)RecieveBuffer, &Time, Longitude, Latitude, &Speed, &currentCOG,&State);
            }
            xSemaphoreGive(DataSemaphore);
            xEventGroupSetBits( FlagsEventGroup,  GPS_ParseFlag );
        }
        else /* xEventGroupWaitBits() returned because of timeout */
        {
        }
    }
}
/***********************************************************************************************
 * Function Name      : GPSRead FreeRTOS Task
 * Description        : Start a new GPS data read Operation.
 * INPUTS             : void* pvParameter
 * RETURNS            : void
 ***********************************************************************************************/
void GPSRead( void* pvParamter){/*Higher Periority Process*/
    EventBits_t uxBits;
    while(1){
        uxBits = xEventGroupWaitBits( FlagsEventGroup, GPS_ParseFlag,  pdTRUE, pdTRUE, timeoutvalue );
        if( ( uxBits & GPS_ParseFlag ) == GPS_ParseFlag )
        {
            GPSGetRawData(RecieveBuffer, Trans_SIZE);
            xEventGroupSetBits( FlagsEventGroup,  GPS_NewReadIssued );
        }
        else /* xEventGroupWaitBits() returned because of timeout */
        {
        }
    }

}
/***********************************************************************************************
 * Function Name      : GPSProcessData FreeRTOS Task
 * Description        : Process the data that is coming from GPS module and prepare the link to be
 *                      transmitted after checking its validity and set the GSM_RateFlag In case a
 *                      modification in the transmission rate to the cloud is required based on the
 *                      movement status
 * INPUTS             : void* pvParameter
 * RETURNS            : void
 ***********************************************************************************************/
void GPSProcessData( void* pvParamter){
    EventBits_t uxBits;
    while(1){
        uxBits = xEventGroupWaitBits( FlagsEventGroup, GPS_NewReadIssued,  pdTRUE, pdTRUE, timeoutvalue );
        //Clear Flag on return
        if( ( uxBits & GPS_NewReadIssued ) == GPS_NewReadIssued )
        {
            //Ensure Atomic Access to the Parsed Data Variables
            if(xSemaphoreTake(DataSemaphore,portMAX_DELAY)){
                if(State=='A' || State=='V' ){
                //if(State=='V' ){ //That is how it should be
                    Sim800PrepareLink(RQSTLink,Longitude, Latitude);
                    //Ensure Atomic Access to the movement Variable
                    if(xSemaphoreTake(MovementSemaphore,portMAX_DELAY)){
                        CMovementStatus=GPSDetectUTurn(currentCOG,Speed);
                        xSemaphoreGive(MovementSemaphore);
                    }else{
                        xSemaphoreGive(MovementSemaphore);
                    }
                    xSemaphoreGive(DataSemaphore);
                    //Set The Validity Flag
                    xEventGroupSetBits( FlagsEventGroup,  GPS_ValidFlag );
                    if(CMovementStatus != PMovementStatus){
                        xEventGroupSetBits( FlagsEventGroup,  GSM_RateFlag );
                        PMovementStatus = CMovementStatus;
                    }else{
                        PMovementStatus = CMovementStatus;
                    }
                    //CHeck State
                    //
                }else{
                    xSemaphoreGive(DataSemaphore);
                }
            }
        }
        else /* xEventGroupWaitBits() returned because of timeout */
        {
        }
    }
}
/***************************************************************************************************************
 * Function Name      : SetTimerRate FreeRTOS Task
 * Description        : Set the Rate in Which the date will be sent to the Network in case new movement status is
 *                      detected on the Movement Status
 * INPUTS             : void* pvParameter
 * RETURNS            : void
 **************************************************************************************************************/
void SetTimerRate( void* pvParamter){
    EventBits_t uxBits;
    while(1){
        uxBits = xEventGroupWaitBits( FlagsEventGroup, GSM_RateFlag, pdTRUE, pdTRUE, timeoutvalue );
        if( ( uxBits & GSM_RateFlag ) == ( GSM_RateFlag) )
        {
            if(xSemaphoreTake(MovementSemaphore,portMAX_DELAY)){
                switch(CMovementStatus){
                case STRAIGHT_LINE:
                    xSemaphoreGive(MovementSemaphore);
                    xTimerChangePeriod( GSMTimer, pdMS_TO_TICKS(60000),pdMS_TO_TICKS(10000) );
                    break;
                case UTURN:
                    xSemaphoreGive(MovementSemaphore);
                    xTimerChangePeriod( GSMTimer, pdMS_TO_TICKS(500),pdMS_TO_TICKS(10000) );
                    break;
                case CURVE:
                    xSemaphoreGive(MovementSemaphore);
                    xTimerChangePeriod( GSMTimer, pdMS_TO_TICKS(5000),pdMS_TO_TICKS(10000) );
                    break;
                }
            }else{
                xSemaphoreGive(MovementSemaphore);
            }
        }

    }
}
/***********************************************************************************************
 * Function Name      : GSMCheckConnection
 * Description        : Check GSM Connection before sending the Data
 * INPUTS             : void* pvParameter
 * RETURNS            : void
 ***********************************************************************************************/
void GSMCheckConnection(void* pvParamter){
    EventBits_t uxBits;
    while(1){
        uxBits = xEventGroupWaitBits( FlagsEventGroup, GPS_ValidFlag|TimerFlag,  pdTRUE, pdTRUE, timeoutvalue );
        //Wait and Clear both Flags on return
        if( ( uxBits & ( GPS_ValidFlag|TimerFlag) ) == ( GPS_ValidFlag|TimerFlag) )
        {
            uint8_t check_cnt=0;
            while(check_cnt<5){
                char *P={0};
                char *err={0};
                uint32_t reslen=5;
                uint32_t comlen=strlen((const char*)EnableHTTPS);
                //The Response buffer is supposed to the size of the transmitted data + received data +20 margin error
                reslen+=20+comlen;
                GSMReceiveResponse(buffer2, reslen);
                GSMSend((uint8_t *)EnableHTTPS,comlen);
                P=strstr((const char*)buffer2,"OK");
                while(*P!='O')
                {
                    P=strstr((const char*)buffer2,"OK");
                    err=strstr((const char*)buffer2,"ERROR");
                    if(*err=='E')
                    {
                        //Reset buffer to recieve new info
                        memset(buffer2,'\0',reslen);
                        break;
                    }
                }
                if(*P=='O')
                {
                    xEventGroupSetBits( FlagsEventGroup,  GSM_ConFlag );
                    //Reset buffer to recieve new info
                    memset(buffer2,'\0',reslen);
                    break;
                }else{
                    //Reset buffer to recieve new info
                    memset(buffer2,'\0',reslen);
                    check_cnt++;
                }
            }
            if(check_cnt==5){
                //Send to EEPROM
            }
        }
        else /* xEventGroupWaitBits() returned because of timeout */
        {
        }
    }
}
/***********************************************************************************************
 * Function Name      : GSMSendSequence
 * Description        : Function to Send a Sequence of Commands In which the data is written to
 *                      the google sheet through the GSM Module
 * INPUTS             : void* pvParameter
 * RETURNS            : void
 ***********************************************************************************************/
void GSMSendSequence(void* pvParamter){
    EventBits_t uxBits;
    while(1){
        uxBits = xEventGroupWaitBits( FlagsEventGroup, GSM_ConFlag,  pdTRUE, pdTRUE, timeoutvalue );
        //Wait and Clear both Flags on return
        if( ( uxBits & ( GSM_ConFlag ) )== ( GSM_ConFlag) )
        {
            uint8_t check_cnt=0,comman_index=0;
            while(comman_index<4){
                char *P={0};
                char *err={0};
                uint32_t reslen=5;
                uint32_t comlen=strlen((const char*)Commands[comman_index]);
                //The Response buffer is supposed to the size of the transmitted data + received data +20 margin error
                reslen+=20+comlen;
                GSMReceiveResponse(buffer2, reslen);
                GSMSend(Commands[comman_index],comlen);
                P=strstr((const char*)buffer2,"OK");
                while(*P!='O')
                {
                    P=strstr((const char*)buffer2,"OK");
                    err=strstr((const char*)buffer2,"ERROR");
                    if(*err=='E')
                    {
                        //Reset buffer to recieve new info
                        memset(buffer2,'\0',reslen);
                        break;
                    }
                }
                if(*P=='O')
                {
                    //Reset buffer to recieve new info
                    memset(buffer2,'\0',reslen);
                    check_cnt=0;
                    comman_index++;
                }else{
                    //Reset buffer to recieve new info
                    memset(buffer2,'\0',reslen);
                    check_cnt++;
                    if(check_cnt==5){
                        //send to EEPROM
                        break;
                    }
                }
            }
        }
        else /* xEventGroupWaitBits() returned because of timeout */
        {
        }
    }
}

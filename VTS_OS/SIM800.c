/******************************************************************************
 * File Name: SIM800.c
 *
 * Description: Source file for SIM800 module integration.
 *
 * Author: AVELABS_D
 *
 * Date : Aug 27 2023
 *******************************************************************************/



/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include "SIM800.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
// Test command to check module communication.
uint8_t     test[5]           =   "AT\r\n";
// Configure module for GPRS connection.
uint8_t     SetCToGPRS[32]    =   "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n";
// Set APN configuration for GPRS connection.
uint8_t     SetAPNCfg[56]     =   "AT+CSTT=\"internet.vodafone.net\",\"internet\",\"internet\"\r\n" ;
// Activate GPRS context.
uint8_t     ActGPRS[15]       =   "AT+SAPBR=1,1\r\n";
// Get assigned IP address.
uint8_t     GetIP[15]         =   "AT+SAPBR=2,1\r\n";
// Initialize HTTP service.
uint8_t     InitHTTP[14]      =   "AT+HTTPINIT\r\n";
// Enable HTTPS for secure communication.
uint8_t     EnableHTTPS[15]   =   "AT+HTTPSSL=1\r\n";
// Set CID parameter for HTTP request.
uint8_t     SetCIDPAR[22]     =   "AT+HTTPPARA=\"CID\",1\r\n";
// Initiate HTTP GET request.
uint8_t     HTTPRequest[136]  =   "AT+HTTPACTION=0\r\n";
// Read HTTP response from the server.
uint8_t     HTTPResponse[18]  =   "AT+HTTPREAD\r\n";
// Terminate HTTP service.
uint8_t     TERMINATEHTTP[14] =   "AT+HTTPTERM\r\n";
// Connect to IP network.
uint8_t     ConnectIP[13]     =    "AT+CGATT=1\r\n";
// Set the URL for the HTTP request.
uint8_t     SetURL[155]       =   "AT+HTTPPARA=\"URL\",\"https://script.google.com/macros/s/AKfycbx8WQYc7m7JuC8h8yKm4WlH8M-6aSU8mOM0s3aCJVzsQ229MBlpJlXxDkZGPEPmBxV-6w/exec?";
// Buffer to store responses and data.
uint8_t buffer2[Sim800BufSize];


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
static uint32_t Sim800SendCommand(uint8_t *Command,char *response);


/*******************************************************************************
 *                              Functions Definitions                           *
 *******************************************************************************/


/***********************************************************************************************
 * Function Name      : Sim800Init
 * Description        : Initializing SIM800 Module
 * INPUTS             : void
 * RETURNS            : void
 ***********************************************************************************************/

void Sim800Init(void)
{
    GSMInit();
	GSMSetReceptionCallBack(sim800recieve);
}

/***********************************************************************************************
 * Function Name      : Sim800SetNetConnectivity
 * Description        : Set up network connectivity for the SIM800 module.
 * INPUTS             : void
 * RETURNS            : uint32_t
 ***********************************************************************************************/
uint32_t Sim800SetNetConnectivity(void)
{
	while(Sim800SendCommand(test,"OK\r\n")!=Gsmok);// Wait for the test command to be successfully executed.
	Sim800SendCommand(SetCToGPRS,"OK\r\n"); // Configure the module for GPRS connection.
	Sim800SendCommand(SetAPNCfg, "OK\r\n"); //Set APN to your network provider
	Sim800SendCommand(ActGPRS,"OK\r\n");    // Set APN to the network provider
	Sim800SendCommand(InitHTTP,"OK\r\n");
	return Gsmok;
}
/***********************************************************************************************
 * Function Name      : Sim800PrepareLink
 * Description        : Prepare the HTTP request link with latitude and longitude.
 * INPUTS             : char *RQSTLink, char *Lon, char *Lat
 * RETURNS            : void
 ***********************************************************************************************/
void Sim800PrepareLink(char *RQSTLink,char *Lon, char *Lat){
    strcpy(RQSTLink,(char *)SetURL); // Copy the base URL for the HTTP request into the RQSTLink buffer.
    strcat(RQSTLink,"lat=");         // Append "lat=" followed by the provided latitude value.
    strcat(RQSTLink,Lat);
    strcat(RQSTLink,"&");            // Append "&" separator between latitude and longitude parameters.
    strcat(RQSTLink,"lon=");
    strcat(RQSTLink,Lon);            // Append "lon=" followed by the provided longitude value.
    strcat(RQSTLink,"\"\r\n");
}
/***********************************************************************************************
 * Function Name      : Sim800SendCommand
 * Description        : Send an AT command to SIM800 module and receive its response.
 * INPUTS             : uint8_t *Command,char *response
 * RETURNS            : uint32_t
 ***********************************************************************************************/
uint32_t Sim800SendCommand(uint8_t *Command,char *response)
{
    char *P={0};
    char *err={0};
    uint32_t reslen =strlen((const char*)response);
    uint32_t comlen =strlen((const char*)Command);
    //The Response buffer is supposed to the size of the transmitted data + received data +20 margin error
    reslen+=comlen+20;
    GSMReceiveResponse(buffer2, reslen);
    GSMSend((uint8_t *)Command,comlen);
    P=strstr((const char*)buffer2,(const char*)response);
    while(*P!=response[0])
    {
        P=strstr((const char*)buffer2,(const char*)response);
        err=strstr((const char*)buffer2,"ERROR\r\n");
        if(*err=='E')
        {
            memset(buffer2,'\0',reslen);
            return GsmError;
        }
    }
    //Reset buffer to recieve new info
    memset(buffer2,'\0',reslen);
    return Gsmok;
}

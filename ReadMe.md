# Vehicle Tracking System

## OverView
Vehicle Tracking System Using Tiva-C. GSM and GPS Modules were used
alongside Tiv-C Peripherals to implement the functionality. FreeRTOS is used to immerse the real Timeness.
The Image Shows The Layered Arch of the system. 
![Alt text](https://github.com/Hsnmhmd/VehicleTrackingSystem/blob/main/SystemLayers.png)

## Function Description
### 1. System Intialization
In this phase the UART1 and Uart2 are intialized to Work with DMA CH 0, 1, 2 To provide the required interfacing for SIM800 and
GPS neo-6m. Further more all the Freertos Tasks,Semaphores and timers are creathed in this phase. Finaly Starting The Scheduler.
### 2.Operation
Once the system is initialized, the DMA Ch 8 starts transferring the GPS data received at UART1 to a buffer in the SRAM to be parsed to extract the required information from the GPRMS frame. The Parsed data is then used to set the rate Upon which the SIM800 Sends the longitude and the latitude to be drawn on a Google map USING Google Locker Studio. The AT Commands and Responses are sent and received using UART2. Transferring the data between SRAM and UART2 is done using DMA Ch 0,1.
![Alt text](https://github.com/Hsnmhmd/VehicleTrackingSystem/blob/main/DataFlowBetweenModules.png)

## Further Info
Chech this Power Point for further information, modes of operation and RTOS Tasks.
https://github.com/Hsnmhmd/VehicleTrackingSystem/blob/main/VTS_NTI_R3_AveLabs_D.pptx
## Requirements and Usage
1. The USage of The Same HW
2. Modify The AT Commands according to your network Provider
3. Adjust the FreeRTOS Timing
4. Modify the HTTPS Link with your Google Apps Script to write to your google sheet or with your desired HTTPS.

## Future Work
Interfacing EEPROM th handle the case of losing the GSM Signal

## Disclaimer
This Project is The Output of a team


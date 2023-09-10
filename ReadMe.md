# Vehicle Tracking System

## OverView
Vehicle Tracking System Using Tiva-C. GSM and GPS Modules were used
alongside Tiv-C Peripherals to implement the functionality. FreeRTOS is used to immerse the real Timeness.

## Function Description
### 1. System Intialization
In this phase the UART1 and Uart2 are intialized to Work with DMA CH 0, 1, 2 To provide the required interfacing for SIM900 and
GPS neo-6m. Further more all the Freertos Tasks,Semaphores and timers are creathed in this phase. Finaly Starting The Scheduler.
### 2.Operation
Once the system initialization is done.The DMA ch 8 starts transfering the data 
## Requirements and Usage
1. Exporting your best .pt yolov5 model to onnx format with batch size =1
2. python 3.9.12 runtime
3. provided requirement.txt
4. provide the path of the images and the connection string of the storage account in the function.json file
5. provide the required connection strings in the local.settings.json
6. modify the __init__.py file with your connection string, your class list and your model
liy

Main prompt: I am working in a project based on health monitoring. The project include the evaluation of a proof of concept device (PoC) to demonstrate that the hardware of B-U585I-IOT02A and a sen0344 sensor can be used to track meaningful health insights (HR, temperature, Spo2, ) of the user including some details of the environment (temperature and pressure). Tha device transmmit the data through BLE to a mobile application that display helth insight to the user in graph and as meanfull information. 

The application also serves as a bridge to azure cloud  specifically iothub that recive the data throught MQTT, using the feature to recibe data directly in the iothub. after the data is recived in the azure cloud is stored in a blob storage for the data to be retrived from Google colab to  process the data and generate some analysis, in this case aggregate data sucha as HRV and RR, data that is derived from the original data collected from the device.


There are two options of report structure for the implementation:

Option1: Chapters arraged by the modules (werable, mobile application, Cloud-Python Scripts)


Option2 uSe the structure above but cretea that flow in the chapter of methodology- implementation- results


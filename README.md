# BLE Module Code - Health Monitoring wearable
## health_monitoring_wearable_BLE
This is the code for a B-U585I-IOT02A board as part of a PoC project for a wearable device to monitor physiological indicators.
The code of the ** BLE MCU ** included in the board for wireless communication and transmit data.

## Data Managed in the BLE

The BLE module has a custom Service in which you can consult the following values:
*** Pacient values ***:
-	Temperature - Temperature
-	HR - Heart Rate
-	SPO2 - Oxygen Saturation

*** Evironmental Values ***
-	Temperature
-	Pressure

The ** data is obtained from the the MAIn MCU STM32U585 ** that is connected to the sensors. The communication between the main MCU and the BLE MCU is done through ** UART1 interface **.

The **CMDs to communicate with the werable through BLE are explained [here](./docs/BLE_CMDs.md)
# PoC Device- Wearable
To implement the health monitoring system a device to collect the data is proposed as part of the implementation. This device is going to be used to evaluate technical fesability and cost of using stm32 MCUs for building of the device. 

## Hardware
For the device the B-U585I-IOT02A board was used as the base to build the proof of concept device, even when some peripherals of the board are not used the design of the system is working with the STM32U585AII6QU Microcontroller for the collection of the data from the internal sensors and external sensor of the board. In addition, the STM32WB5MMGH6TR is used for the BLE commmunication, and the data is thranmitted from the fist MCU using a USAr

### Sensors 
The sensors used for the data collection and the data collected are listed below

### Patient 


#### Environment
All the sensors used to measure environmental variables are included in the board and are **wired to the STM32U585AII6QU MCU using the I2C interface**, specifically the I2C2_SCL and I2C_SDA pins. 

##### Environmental Temperature 
***HTS221- Relative Humidity and temperature sensor**
-Communication available: SPI & I2C communication.
-Id placement: U28

For the I2C communication the address used are: 
-   Read=10111111 (BFh)
-   **Write=10111110 (BEh)**

##### Pressure Sensor
***LPS22HH- Relative Humidity and temperature sensor**

Pressure sensor

--
-Communication available:  I2C communication.
-Id placement: U26

For the I2C communication the address used are: 
- Read=10111011(BBh)
- **Write=10111010(BAh)**

## Main MCU- Dta collection and communication with the sensors 
In this report the main mcu of  the werable device refers to the STM32U585AII6QU that is a low power MCU od the U5 family of 


## BLE communication
The aplication of the code implement 

### Security of the communication


### Packaging and Msg 
In here we should refer to this part [BLE CMds docummentation ](./BLE_CMDS.md) 




## Comments for the professors

**
*Should i include here the [BLE CMds docummentation ](./BLE_CMDS.md) or in an Anexo-> maybe i should include a reduced version*





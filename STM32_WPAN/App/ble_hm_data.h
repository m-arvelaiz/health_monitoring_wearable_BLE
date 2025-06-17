/*
 * ble_hm_data.h
 *
 *  Created on: May, 2025
 *      Author: Mariana Arvelaiz
 */

#ifndef APP_BLE_HM_DATA_H_
#define APP_BLE_HM_DATA_H_

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "custom_stm.h"
#define BLE_DEBUG_DUMMY_DATA

/* USER CODE END Includes */


typedef enum {
    CMD_REQ_HR_SPO2_DATA      = 0x01, // HR & SpO2 + timestamp
    CMD_REQ_TEMP_DATA         = 0x02, // Temperature + timestamp
    CMD_REQ_PRESSURE_DATA     = 0x03, // Pressure + timestamp
    CMD_REQ_ALL_DATA          = 0x04, // HR, SpO2, Temp, Pressure + timestamp
    CMD_REQ_HISTORICAL_DATA   = 0x10, // Request historical data from timestamp
    CMD_SET_UNIX_TIME         = 0x20, // Set device time
    CMD_SET_SENSOR_CONFIG     = 0x30, // Configure sampling or future features
    CMD_START_STREAM          = 0x40, // Begin live sensor stream
    CMD_STOP_STREAM           = 0x41, // Stop live stream
    CMD_UNKNOWN               = 0xFF  // Unknown or unsupported command
} BLE_CmdID_t;



typedef enum {
	Notify_None,
	Notify_Pending
} BLE_NotifyChar_Status;

/* USER CODE BEGIN ET */



typedef struct
{
  BLE_NotifyChar_Status	Status;
  uint8_t*	pck;
  uint8_t	checkSum;
  uint8_t	length;
} Custom_BLE_Notify_interface_t;

Custom_BLE_Notify_interface_t* ble_notify_interface_get();

#endif /* APP_BLE_HM_DATA_H_ */

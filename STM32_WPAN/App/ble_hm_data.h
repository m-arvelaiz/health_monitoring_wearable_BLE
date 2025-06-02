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
#include "custom_stm.h"


/* USER CODE END Includes */


typedef enum {
	Notify_None,
	Notify_Pending
} BLE_NotifyChar_Status;

/* USER CODE BEGIN ET */
typedef struct
{
  uint8_t	pck_id;
  uint8_t	type;
  uint8_t	data [8];
  uint8_t	checkSum;
  uint8_t	pck[11];
  uint8_t	length;
} Custom_BLE_Payload_t;


typedef struct
{
  BLE_NotifyChar_Status	Status;
  uint8_t*	pck;
  uint8_t	checkSum;
  uint8_t	length;
} Custom_BLE_Notify_interface_t;

#endif /* APP_BLE_HM_DATA_H_ */

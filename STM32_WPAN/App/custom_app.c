/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ble_hm_data.h"
#include "ble_custom_utils.h"
#include "data_handler.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* HealthMonitoringS */
  uint8_t               Charnotify_Notification_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */

  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

uint8_t UpdateCharData[512];
uint8_t NotifyCharData[512];
uint16_t Connection_Handle;
/* USER CODE BEGIN PV */
uint8_t flag_notify=0x00;
Custom_BLE_Notify_interface_t Notify_Interface;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* HealthMonitoringS */
static void Custom_Charnotify_Update_Char(void);
static void Custom_Charnotify_Send_Notification(void);

/* USER CODE BEGIN PFP */
void myTask(void){
	//This task Controls notifycation Char. Check Pending Notification and execute it
	if(Notify_Interface.Status==Notify_Pending){
		Notify_Interface.Status=Notify_None;
//		UpdateCharData[0]+=0x01;


//		Custom_Charnotify_Update_Char(); // This function triggers the Notificaation of the the content of NotifyCharData array
		Custom_Charnotify_Send_Notification();
	}


	//Re-Schedule the task
	UTIL_SEQ_SetTask(1<<CFG_TASK_MY_TASK, CFG_SCH_PRIO_0);
}


Custom_BLE_Notify_interface_t* ble_notify_interface_get(){
	return &Notify_Interface;
}
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* HealthMonitoringS */
    case CUSTOM_STM_CHARWRITE_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CHARWRITE_WRITE_EVT */

    	uint8_t * p_Payload = pNotification->DataTransfered.pPayload ;
    	uint8_t req_length = pNotification->DataTransfered.Length ;

    	// Sanity check for minimal packet size (CMD + LEN + CRC)
//		if (req_length < 7)
//			break;
		uint8_t cmd = p_Payload[0];
		uint8_t len = p_Payload[1];
		uint8_t *params = &p_Payload[2];
		uint8_t chksum_rcv = p_Payload[req_length - 1];
		uint8_t chksum_calc = ble_calculate_chksum(p_Payload, req_length - 1);

//		if (chksum_rcv != chksum_calc) {
//			// CRC mismatch – discard or respond with error
//			break;
//		}


		// Filter the CMds

		    if (cmd == CMD_REQ_HR_SPO2_DATA) { // REQ_HR_SPO2_DATA
		    	data_handler_req_hr_spo2();

		    } else if (cmd == CMD_REQ_TEMP_DATA) { // REQ_TEMP_DATA
		        uint8_t temp_type = params[0];  // 0x01 = Body, 0x02 = Env, 0x03 = Both
		        data_handler_req_temp(temp_type);

		    } else if (cmd == CMD_REQ_PRESSURE_DATA) { // REQ_PRESSURE_DATA
		    	data_handler_req_pressure();

		    } else if (cmd == 0x04) { // REQ_ALL_DATA
		    	data_handler_req_all_data();

		    } else if (cmd == 0x10) { // REQ_HISTORICAL_DATA
		        uint32_t start_time = (params[0] << 24) | (params[1] << 16) | (params[2] << 8) | params[3];
		        data_handler_req_historical_data(start_time);

		    } else if (cmd == 0x20) { // SET_UNIX_TIME
		        uint32_t unix_time = (params[0] << 24) | (params[1] << 16) | (params[2] << 8) | params[3];
		        data_handler_req_set_unix_time(unix_time);

		    } else if (cmd == 0x30) { // SET_SENSOR_CONFIG
		        uint8_t sensor_type = params[0]; // Igual al tipo de dato (HR, SpO2, Temp, etc.)
		        uint8_t config_val  = params[1]; // Valor de configuración (más detalles en el futuro)
		        data_handler_req_set_sensor_config(sensor_type, config_val);

		    } else if (cmd == 0x40) { // START_STREAM
		        uint8_t stream_mask = params[0];
		        data_handler_req_start_stream(stream_mask);

		    } else if (cmd == 0x41) { // STOP_STREAM
		    	data_handler_req_stop_stream();

		    } else {
		        // Comando desconocido
		    }



//    	Notify_Interface.Status=Notify_Pending; // Schedule a new Notification

      /* USER CODE END CUSTOM_STM_CHARWRITE_WRITE_EVT */
      break;

    case CUSTOM_STM_CHARNOTIFY_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CHARNOTIFY_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_CHARNOTIFY_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_CHARNOTIFY_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CHARNOTIFY_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_CHARNOTIFY_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_NOTIFICATION_COMPLETE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_NOTIFICATION_COMPLETE_EVT */

      /* USER CODE END CUSTOM_STM_NOTIFICATION_COMPLETE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

      /* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */

  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

    /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
    case CUSTOM_CONN_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */

      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

      /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_APP_Notification_default */

      /* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
  /* USER CODE BEGIN CUSTOM_APP_Init */
	Notify_Interface.Status=Notify_None;
	Notify_Interface.checkSum=0;
	Notify_Interface.length=0;
//	Notify_Interface.pck=UpdateCharData;
	Notify_Interface.pck=NotifyCharData;

  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* HealthMonitoringS */
__USED void Custom_Charnotify_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Charnotify_UC_1*/
  updateflag=1;

  /* USER CODE END Charnotify_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_CHARNOTIFY, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Charnotify_UC_Last*/

  /* USER CODE END Charnotify_UC_Last*/
  return;
}

void Custom_Charnotify_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Charnotify_NS_1*/
  updateflag=1;
  /* USER CODE END Charnotify_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_CHARNOTIFY, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Charnotify_NS_Last*/

  /* USER CODE END Charnotify_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/

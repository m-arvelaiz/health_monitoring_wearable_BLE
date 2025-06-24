/*
 * data_handle.h
 *
 *  Created on: Jun 3, 2025
 *      Author: magaa
 */

#ifndef APP_DATA_HANDLER_H_
#define APP_DATA_HANDLER_H_

#include <stdint.h>
#include "ble_hm_data.h"


// -----------------------------------------------------------------------------
// Prototypes for “handle_*” functions called by the BLE write‐handler.
// Each of these will package a UART request to the secondary MCU and then
// process the reply.
// -----------------------------------------------------------------------------

/**
 * @brief  Request current Heart Rate & SpO₂ (no parameters).
 *         Internally, this function sends a UART frame to request HR/SpO₂.
 */
void data_handler_req_hr_spo2(void);


void data_handler_notify_hr_spo2(void);

/**
 * @brief  Request current Temperature.
 * @param  temp_type: 0x01 = Body temp, 0x02 = Env temp, 0x03 = Both
 *         Internally, this function sends a UART frame with temp_type.
 */
void data_handler_req_temp(uint8_t temp_type);

void data_handler_notify_temp();

/**
 * @brief  Request current Pressure (no parameters).
 *         Internally, this function sends a UART frame to request pressure.
 */
void data_handler_req_pressure(void);

void data_handler_notify_pressure();
/**
 * @brief  Request all sensor data (HR, SpO₂, Temp, Pressure).
 *         Internally, this function sends a UART frame to retrieve all sensors.
 */
void data_handler_req_all_data(void);

/**
 * @brief  Request historical data starting from a given Unix timestamp.
 * @param  start_time: 32-bit Unix timestamp (big-endian).
 *         Internally, this function sends a UART frame with start_time.
 */
void data_handler_req_historical_data(uint32_t start_time);

/**
 * @brief  Set the current Unix time on the secondary MCU.
 * @param  unix_time: 32-bit Unix time (big-endian).
 *         Internally, this function sends a UART frame to synchronize clocks.
 */
void data_handler_req_set_unix_time(uint32_t unix_time);

void data_handler_notify_unix_time();
/**
 * @brief  Configure a sensor’s sampling rate or feature set.
 * @param  sensor_type: 1-byte ID (same as BLE data type).
 * @param  config_val:  1-byte configuration value (TBD).
 *         Internally, this function sends a UART frame to configure that sensor.
 *         NOTE: For now all the sensor will have the same sample time configure through here
 */
void data_handler_req_set_sensor_config(uint8_t sensor_type, uint8_t config_val);
void data_handler_notify_set_sensor_config();
/**
 * @brief  Begin streaming live data.
 * @param  stream_mask: Bitmask (e.g., 0x1F = HR | SpO₂ | Temp | Pressure | …).
 *         Internally, this function sends a UART frame to start streaming.
 */
void data_handler_req_start_stream(uint8_t stream_mask);
void data_handler_notify_start_stream();
void data_handler_notify_data_stream();
/**
 * @brief  Stop any ongoing live-streaming on the secondary MCU (no parameters).
 *         Internally, this function sends a UART frame to stop streaming.
 */
void data_handler_req_stop_stream(void);
void data_handler_notify_stop_stream();

#define DATA_HANDLER_PAYLOAD_LENGHT 10

typedef struct Data_Handler {
	BLE_CmdID_t data_cmd;
    uint8_t* payload;
} Data_Handler_t;


Data_Handler_t* data_handler_get(void);
void data_handler_DeInit(void) ;
void data_handler_Init();

void data_handler_dispatcher(uint8_t cmd, uint8_t* payload, uint8_t payload_len);

#endif /* APP_DATA_HANDLER_H_ */

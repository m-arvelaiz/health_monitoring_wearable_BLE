/*
 * data_handle.c
 *
 *  Created on: Jun 3, 2025
 *      Author: magaa
 */


#include "data_handler.h"
#include "ble_custom_utils.h"
#include "ble_hm_data.h"
#include <string.h>

#ifdef BLE_DEBUG_DUMMY_DATA
/**
 * @brief  Generate and send a dummy HR & SpO₂ notification over BLE.
 *         Structure: [CMD][LEN][DATA(5)][TIMESTAMP(4)][CRC]
 */
static void send_dummy_hr_spo2(void)
{
	Custom_BLE_Notify_interface_t* p_ble_notify =ble_notify_interface_get();
    uint8_t* notif= p_ble_notify->pck ;
    uint8_t idx = 0;

    // DATA_TYPE: bitmask for HR (0x01) + SpO₂ (0x02) = 0x03
	notif[idx++] = 0x01;
	// LEN: always 0x05
	notif[idx++] = 0x05;

	// DATA: HR (2 bytes), SpO₂ (2 bytes), padding (1 byte)
	uint16_t dummy_hr = 70;    // e.g., 70 bpm -> 0x0046
	uint16_t dummy_spo2 = 98;  // e.g., 98% -> 0x0062

	notif[idx++] = 0x00;       // Padding
	notif[idx++] = (dummy_hr >> 8) & 0xFF;
	notif[idx++] = (dummy_hr >> 0) & 0xFF;
	notif[idx++] = (dummy_spo2 >> 8) & 0xFF;
	notif[idx++] = (dummy_spo2 >> 0) & 0xFF;

    // TIMESTAMP: fixed dummy Unix time, e.g., 1620000000 (0x60D4A000)
    uint32_t ts = 0x60D4A000;
    notif[idx++] = (ts >> 24) & 0xFF;
    notif[idx++] = (ts >> 16) & 0xFF;
    notif[idx++] = (ts >> 8)  & 0xFF;
    notif[idx++] = (ts >> 0)  & 0xFF;

    // CRC: sum of bytes [0..10] mod 256
    notif[idx] = ble_calculate_chksum(notif, 11);


    p_ble_notify->Status=Notify_Pending;
}

/**
 * @brief  Generate and send a dummy Temperature notification over BLE.
 * @param  temp_type: 0x01 = Body, 0x02 = Environmental, 0x03 = Both
 */
static void send_dummy_temp(uint8_t temp_type)
{
	Custom_BLE_Notify_interface_t* p_ble_notify = ble_notify_interface_get();
	uint8_t *notif = p_ble_notify->pck;
	uint8_t idx = 0;

    // DATA_TYPE: bitmask for Temperature = 0x04
    notif[idx++] = 0x02;
    // LEN: always 0x05
    notif[idx++] = 0x05;

    // DATA: depends on temp_type
    // Use dummy values: Ambient = 25.0°C -> 250 (0x00FA), Body = 36.5°C -> 365 (0x016D)
    uint16_t amb = 250;
    uint16_t body = 365;

    if (temp_type == 0x01) {
        // Body only
        notif[idx++] = 0x01;
        notif[idx++] = (body >> 8) & 0xFF;
        notif[idx++] = (body >> 0) & 0xFF;
        notif[idx++] = 0x00;
        notif[idx++] = 0x00; // Padding
    } else if (temp_type == 0x02) {
        // Ambient only
        notif[idx++] = 0x02;
        notif[idx++] = (amb >> 8) & 0xFF;
        notif[idx++] = (amb >> 0) & 0xFF;
        notif[idx++] = 0x00;
        notif[idx++] = 0x00; // Padding
    } else {
        // Both: pack ambient then body, pad 1 byte
    	notif[idx++] = 0x03; // Padding
        notif[idx++] = (amb >> 8) & 0xFF;
        notif[idx++] = (amb >> 0) & 0xFF;
        notif[idx++] = (body >> 8) & 0xFF;
        notif[idx++] = (body >> 0) & 0xFF;

    }

    // TIMESTAMP: fixed dummy Unix time
    uint32_t ts = 0x60D4A000;
    notif[idx++] = (ts >> 24) & 0xFF;
    notif[idx++] = (ts >> 16) & 0xFF;
    notif[idx++] = (ts >> 8)  & 0xFF;
    notif[idx++] = (ts >> 0)  & 0xFF;

    // CRC: sum of bytes [0..10] mod 256
    notif[idx] = ble_calculate_chksum(notif, 11);

    p_ble_notify->Status=Notify_Pending;
}

/**
 * @brief  Generate and send a dummy Pressure notification over BLE.
 */
static void send_dummy_pressure(void)
{
	Custom_BLE_Notify_interface_t* p_ble_notify = ble_notify_interface_get();
	uint8_t *notif = p_ble_notify->pck;
    uint8_t idx = 0;

    // DATA_TYPE: bitmask for Pressure = 0x08
    notif[idx++] = 0x03;
    // LEN: always 0x05
    notif[idx++] = 0x05;

    // DATA: Pressure (e.g., 1013 hPa -> 0x03F5), pad 3 bytes
    uint16_t dummy_press = 1013;
    notif[idx++] = 0x00;
    notif[idx++] = (dummy_press >> 8) & 0xFF;
    notif[idx++] = (dummy_press >> 0) & 0xFF;
    notif[idx++] = 0x00;
    notif[idx++] = 0x00; // Padding

    // TIMESTAMP: fixed dummy Unix time
    uint32_t ts = 0x60D4A000;
    notif[idx++] = (ts >> 24) & 0xFF;
    notif[idx++] = (ts >> 16) & 0xFF;
    notif[idx++] = (ts >> 8)  & 0xFF;
    notif[idx++] = (ts >> 0)  & 0xFF;

    // CRC: sum of bytes [0..10] mod 256
    notif[idx] = ble_calculate_chksum(notif, 11);

    p_ble_notify->Status=Notify_Pending;
}

/**
 * @brief  Generate and send dummy “all data” notifications over BLE.
 *         Splits into two notifications: [HR+SpO₂], then [Temp+Pressure].
 */
static void send_dummy_all_data(void)
{
    // First packet: HR (0x01) + SpO₂ (0x02) = data_type 0x03
//    {
		Custom_BLE_Notify_interface_t* p_ble_notify =ble_notify_interface_get();
		uint8_t *notif = p_ble_notify->pck;
        uint8_t idx1 = 0;
        notif[idx1++] = 0x04;  // DATA_TYPE
        notif[idx1++] = 0x05;  // LEN

        uint16_t dummy_hr = 70;
        uint16_t dummy_spo2 = 98;

        notif[idx1++] = 0x00;  // Padding
        notif[idx1++] = (dummy_hr >> 8) & 0xFF;
        notif[idx1++] = (dummy_hr >> 0) & 0xFF;
        notif[idx1++] = (dummy_spo2 >> 8) & 0xFF;
        notif[idx1++] = (dummy_spo2 >> 0) & 0xFF;


        uint32_t ts = 0x60D4A000;
        notif[idx1++] = (ts >> 24) & 0xFF;
        notif[idx1++] = (ts >> 16) & 0xFF;
        notif[idx1++] = (ts >> 8)  & 0xFF;
        notif[idx1++] = (ts >> 0)  & 0xFF;

        notif[idx] = ble_calculate_chksum(notif, 11);
        p_ble_notify->Status=Notify_Pending;
//    }
//
//    // Second packet: Temp (0x04) + Pressure (0x08) = data_type 0x0C
//    {
//        uint8_t notif2[12];
//        uint8_t idx2 = 0;
//        notif2[idx2++] = 0x0C;  // DATA_TYPE
//        notif2[idx2++] = 0x05;  // LEN
//
//        uint16_t amb = 250;     // 25.0°C
//        uint16_t body = 365;    // 36.5°C
//        uint16_t dummy_press = 1013;
//
//        // Pack first two readings: ambient + body
//        notif2[idx2++] = (amb >> 8) & 0xFF;
//        notif2[idx2++] = (amb >> 0) & 0xFF;
//        notif2[idx2++] = (body >> 8) & 0xFF;
//        notif2[idx2++] = (body >> 0) & 0xFF;
//        notif2[idx2++] = 0x00;  // Padding (only two readings fit)
//
//        uint32_t ts = 0x60D4A000;
//        notif2[idx2++] = (ts >> 24) & 0xFF;
//        notif2[idx2++] = (ts >> 16) & 0xFF;
//        notif2[idx2++] = (ts >> 8)  & 0xFF;
//        notif2[idx2++] = (ts >> 0)  & 0xFF;
//
//        notif[idx] = ble_calculate_chksum(notif, 11);
//        p_ble_notify->Status=Notify_Pending;

        // If you want a separate Pressure-only packet, uncomment below:
        /*
        uint8_t notif3[12];
        uint8_t idx3 = 0;
        notif3[idx3++] = 0x08;  // DATA_TYPE = Pressure
        notif3[idx3++] = 0x05;  // LEN

        notif3[idx3++] = (dummy_press >> 8) & 0xFF;
        notif3[idx3++] = (dummy_press >> 0) & 0xFF;
        notif3[idx3++] = 0x00;
        notif3[idx3++] = 0x00;
        notif3[idx3++] = 0x00;

        notif3[idx3++] = (ts >> 24) & 0xFF;
        notif3[idx3++] = (ts >> 16) & 0xFF;
        notif3[idx3++] = (ts >> 8)  & 0xFF;
        notif3[idx3++] = (ts >> 0)  & 0xFF;

        notif3[idx3] = calc_crc(notif3, 11);
        send_ble_notification(notif3, sizeof(notif3));
//        */
//    }
}
#endif // BLE_DUMMY



/**
 * @brief  Request current Heart Rate & SpO₂ + timestamp from the other MCU.
 */
void data_handler_req_hr_spo2(void)
{
#ifdef BLE_DEBUG_DUMMY_DATA
    send_dummy_hr_spo2();
#else
    uint8_t params[4] = {0x00, 0x00, 0x00, 0x00};
    uart_send_frame(0x01, params);
    uart_receive_response();
#endif
}

/**
 * @brief  Request Temperature.
 * @param  temp_type: 0x01 = Body, 0x02 = Environmental, 0x03 = Both
 */
void data_handler_req_temp(uint8_t temp_type)
{
#ifdef BLE_DEBUG_DUMMY_DATA
    send_dummy_temp(temp_type);
#else
    uint8_t params[4] = { temp_type, 0x00, 0x00, 0x00 };
    uart_send_frame(0x02, params);
    uart_receive_response();
#endif
}

/**
 * @brief  Request Pressure.
 */
void data_handler_req_pressure(void)
{
#ifdef BLE_DEBUG_DUMMY_DATA
    send_dummy_pressure();
#else
    uint8_t params[4] = {0x00, 0x00, 0x00, 0x00};
    uart_send_frame(0x03, params);
    uart_receive_response();
#endif
}

/**
 * @brief  Request all sensor data (HR, SpO₂, Temp, Pressure).
 */
void data_handler_req_all_data(void)
{
#ifdef BLE_DEBUG_DUMMY_DATA
    send_dummy_all_data();
#else
    uint8_t params[4] = {0x00, 0x00, 0x00, 0x00};
    uart_send_frame(0x04, params);
    uart_receive_response();
#endif
}

/**
 * @brief  Request historical data starting from a Unix timestamp.
 * @param  start_time: 32-bit Unix timestamp (big-endian).
 */
void data_handler_req_historical_data(uint32_t start_time)
{
    uint8_t params[4];
    params[0] = (start_time >> 24) & 0xFF;
    params[1] = (start_time >> 16) & 0xFF;
    params[2] = (start_time >> 8)  & 0xFF;
    params[3] = (start_time >> 0)  & 0xFF;

//    uart_send_frame(0x10, params);
//    uart_receive_response();
    // Note: might require multiple responses if there are many historical records.
}

/**
 * @brief  Send current Unix time to the other MCU.
 * @param  unix_time: 32-bit Unix time (big-endian).
 */
void data_handler_req_set_unix_time(uint32_t unix_time)
{
    uint8_t params[4];
    params[0] = (unix_time >> 24) & 0xFF;
    params[1] = (unix_time >> 16) & 0xFF;
    params[2] = (unix_time >> 8)  & 0xFF;
    params[3] = (unix_time >> 0)  & 0xFF;

//    uart_send_frame(0x20, params);
//    uart_receive_response();
}

/**
 * @brief  Configure a sensor (sampling rate or future features).
 * @param  sensor_type: 1-byte ID (same as BLE data type).
 * @param  config_val:  1-byte configuration value (TBD).
 */
void data_handler_req_set_sensor_config(uint8_t sensor_type, uint8_t config_val)
{
    uint8_t params[4] = { sensor_type, config_val, 0x00, 0x00 };
//    uart_send_frame(0x30, params);
//    uart_receive_response();
}

/**
 * @brief  Start live data streaming.
 * @param  stream_mask: bitmask (e.g., 0x1F = HR | SpO₂ | Temp | Pressure).
 */
void data_handler_req_start_stream(uint8_t stream_mask)
{
    uint8_t params[4] = { stream_mask, 0x00, 0x00, 0x00 };
//    uart_send_frame(0x40, params);
//    // The secondary MCU will begin sending periodic frames:
//    // [START][DATA_TYPE][LEN][DATA...][TIMESTAMP][CRC]
//    uart_receive_response();
}

/**
 * @brief  Stop any ongoing live streaming.
 */
void data_handler_req_stop_stream(void)
{
    uint8_t params[4] = {0x00, 0x00, 0x00, 0x00};
//    uart_send_frame(0x41, params);
//    uart_receive_response();
}

/*
 * ble_custom_utils.h
 *
 *  Created on: Jun 3, 2025
 *      Author: magaa
 */

#ifndef APP_BLE_CUSTOM_UTILS_H_
#define APP_BLE_CUSTOM_UTILS_H_

#include <stdint.h>

uint8_t ble_calculate_chksum(uint8_t *pck, uint8_t length);

#endif /* APP_BLE_CUSTOM_UTILS_H_ */

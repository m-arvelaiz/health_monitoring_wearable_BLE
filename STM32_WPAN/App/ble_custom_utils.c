/*
 * ble_custom_utils.c
 *
 *  Created on: Jun 3, 2025
 *      Author: magaa
 */

#include "ble_custom_utils.h"



uint8_t ble_calculate_chksum(uint8_t *pck, uint8_t length){
	uint8_t chksum=0x00;
	uint16_t i;


	for(i=0; i<length; i++){
		chksum= (*(pck+i)) +chksum;
	}

	return chksum;
}

/*
 * uart_handler.c
 *
 *  Created on: Jun 16, 2025
 *      Author: magaa
 */


#include "uart_handler.h"

#include <stdlib.h>
#include <string.h>
#include <main.h>

static UART_Handler_t *uart_handler = NULL;


static uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
static uint8_t rx_buffer[UART_RX_BUFFER_SIZE];


static void uart_handler_Process_Received_pck(uint8_t* pck, uint16_t size);
static void uart_handler_Send_Cmd(UART_CommandType_t cmd, uint8_t* payload, uint8_t payload_len);


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){

	//Check size and the first byte
	if (Size >=7) {

		if (uart_handler->rx_buffer==0xAA){

			uart_handler->process_recived_pck(uart_handler->rx_buffer, Size);
		}

	 }


	HAL_UARTEx_ReceiveToIdle_IT(uart_handler->huart, uart_handler->rx_buffer, 32);

}

static uint8_t Calculate_CRC(const uint8_t *data, uint8_t length) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < length; ++i) {
        crc ^= data[i];
    }
    return crc;
}

// Local functions (assigned as function pointers)
static void uart_handler_Process_Received_pck(uint8_t* pck, uint16_t size) {
	//Check CRC
	uint8_t actual_crc= *(uart_handler->rx_buffer+size);
	uint8_t calculated_crc=Calculate_CRC(uart_handler->rx_buffer, size);
    if (actual_crc == calculated_crc) {

    }


}

static void uart_handler_Send_Cmd(UART_CommandType_t cmd, uint8_t* payload, uint8_t payload_len) {

    uint8_t index = 0;

    uart_handler->tx_buffer[index++] = 0xAA;
    uart_handler->tx_buffer[index++] = cmd;
    uart_handler->tx_buffer[index++] = payload_len;

    memcpy(&uart_handler->tx_buffer[index], payload, payload_len);
    index += payload_len;



    uint8_t crc = Calculate_CRC(uart_handler->tx_buffer, index);
    uart_handler->tx_buffer[index++] = crc;

    HAL_UART_Transmit(uart_handler->huart, uart_handler->tx_buffer, index, HAL_MAX_DELAY); /// NOTE: Check if this  is cerrectly send here or i shoudl check this in a timer
    uart_handler->state = UART_STATE_IDLE;
}

void uart_handler_Init(UART_HandleTypeDef *huart) {




    uart_handler = (UART_Handler_t *)malloc(sizeof(UART_Handler_t));

    uart_handler->huart = huart;
    uart_handler->cmd_packet = (UART_CommandPacket_t *)malloc(sizeof(UART_CommandPacket_t));
    uart_handler->resp_packet = (UART_ResponsePacket_t *)malloc(sizeof(UART_ResponsePacket_t));

    uart_handler->state = UART_STATE_IDLE;
    uart_handler->max_payload_len = UART_RX_BUFFER_SIZE - 6;

    uart_handler->process_recived_pck = uart_handler_Process_Received_pck;
    uart_handler->send_cmd = uart_handler_Send_Cmd;
    uart_handler->tx_buffer=tx_buffer;
    uart_handler->rx_buffer=rx_buffer;

    HAL_UARTEx_ReceiveToIdle_IT(uart_handler->huart, uart_handler->rx_buffer, 32);

    memset(uart_handler->cmd_packet, 0, sizeof(UART_CommandPacket_t));
    memset(uart_handler->resp_packet, 0, sizeof(UART_ResponsePacket_t));
}

void uart_handler_DeInit(void) {
    if (uart_handler != NULL) {
        free(uart_handler->cmd_packet);
        free(uart_handler->resp_packet);
        free(uart_handler);
        uart_handler = NULL;
    }

}

void uart_handler_Reset_State(void) {
    if (uart_handler) {
        uart_handler->state = UART_STATE_IDLE;

        memset(uart_handler->cmd_packet, 0, sizeof(UART_CommandPacket_t));
        memset(uart_handler->resp_packet, 0, sizeof(UART_ResponsePacket_t));
    }
}

UART_Handler_t* uart_handler_get(void) {
    return uart_handler;
}

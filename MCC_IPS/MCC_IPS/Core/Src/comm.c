/*
 * comm.c
 *
 *  Created on: Mar 24, 2024
 *      Author: amay
 */


#include "main.h"
#include "comm.h"
#include "speed.h"
#include "current.h"
#include "usart.h"
#include "pwmCommand.h"
#include <stdio.h>

//====================================================================
//			SIZE
//====================================================================

#define ID_CHAR_SIZE 4
#define FLOAT_CHAR_SIZE 16
#define INT_CHAR_SIZE 8
#define UINT32_CHAR_SIZE 16
#define UINT16_CHAR_SIZE 8

#define RX_BUFFER_SIZE 	(int)32

//====================================================================
//			RX IDENTIFIERS TABLE
//====================================================================
//ALL UNSIGNED CHAR
#define ID_RX_START			(unsigned char)0x01
#define ID_RX_STOP			(unsigned char)0x02
#define ID_RX_PWM_START		(unsigned char)0x03
#define ID_RX_PWM_STOP		(unsigned char)0x04
#define ID_RX_DUTY_SET		(unsigned char)0x05
#define ID_RX_DIR			(unsigned char)0x06
#define ID_RX_VOLTAGE_SET  	(unsigned char)0x07

#define ID_RX_SEND_KP		(unsigned char)0x08
#define ID_RX_SET_KP		(unsigned char)0x09

#define ID_RX_SEND_KP		(unsigned char)0x0A
#define ID_RX_SET_KP		(unsigned char)0x0B
#define ID_RX_SEND_KI		(unsigned char)0x0C
#define ID_RX_SET_KI		(unsigned char)0x0D
#define ID_RX_SEND_KD		(unsigned char)0x0E
#define ID_RX_SET_KD		(unsigned char)0x0F

//====================================================================
//			TX IDENTIFIERS TABLE
//====================================================================
//ALL UNSIGNED CHAR
//ID size = 4 chars
#define ID_TX_TIME			(unsigned int)0x01
#define ID_TX_ADC			(unsigned int)0x10
#define ID_TX_PWM			(unsigned int)0x11
#define ID_TX_RPM			(unsigned int)0x12
#define ID_TX_DIR			(unsigned int)0x13


extern void dyn_rcv_cb(uint8_t car);

unsigned char rx_data[RX_BUFFER_SIZE];

void comm_init(void)
{
	/* USART RECEPTION */
	HAL_USART_Receive_IT(&husart2, rx_data, RX_BUFFER_SIZE);
}


//====================================================================
//			USART TX
//====================================================================

void comm_SendData(void)
{
	//DATA_FORMAT : tx_data[x] = [ID_TX_TypeOfData]+[TypeOfData_0]+...+[TypeOfData_n]+[ID_TX_TypeOfData...]+...

	//printf("\n\r");
	/* TIME */
	printf("0x%x;", ID_TX_TIME); //hex id
	printf("%i;", (int)HAL_GetTick());
	/* ADC */
	printf("0x%x;", ID_TX_ADC); //hex id
	printf("%f;", ADC_LastValue);
	/* PWM */
	printf("0x%x;", ID_TX_PWM); //hex id
	printf("%u;", (unsigned int)PWM_Value); //uchar typecast for warning
	/* PWM */
	printf("0x%x;", ID_TX_RPM); //hex id
	printf("%f;", output_speed);

	printf("\n\r");
}


//====================================================================
//			USART RX
//====================================================================

void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{
	/*
	 * Protocole à la réception :
	 *
	 * 1. Traitement des données reçues
	 *
	 * 2. Réarmement de la réception en interruption des données
	 *
	 */

	//Traitement données


	//Réarmement réception
	//HAL_StatusTypeDef HAL_USART_Receive_IT(USART_HandleTypeDef *husart, uint8_t *pRxData, uint16_t Size)
	HAL_USART_Receive_IT(&husart2, rx_data, RX_BUFFER_SIZE);
}

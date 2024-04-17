/*
 * comm.c
 *
 *  Created on: Mar 24, 2024
 *      Author: madec
 */


#include "main.h"
#include "comm.h"
#include "speed.h"
#include "current.h"
#include "usart.h"
#include "pwmCommand.h"
#include "pidFilter.h"
#include "mccCommand.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//====================================================================
//			SIZE
//====================================================================

#define ID_CHAR_SIZE 4
#define FLOAT_CHAR_SIZE 16
#define INT_CHAR_SIZE 8
#define UINT32_CHAR_SIZE 16
#define UINT16_CHAR_SIZE 8

#define RX_BUFFER_SIZE 	(int)16

//====================================================================
//			RX IDENTIFIERS TABLE
//====================================================================
//ALL UNSIGNED CHAR
#define ID_RX_START			"0x01"
#define ID_RX_STOP			"0x02"
#define ID_RX_PWM_START		"0x03"
#define ID_RX_PWM_STOP		"0x04"
#define ID_RX_DUTY_SET		"0x05"
#define ID_RX_DIR			"0x06"
#define ID_RX_VOLTAGE_SET  	"0x07"

/*
#define ID_RX_SEND_KP		0x08
#define ID_RX_SET_KP		0x09
*/

#define ID_RX_SEND_KP		"0x0A"
#define ID_RX_SET_KP		"0x0B"
#define ID_RX_SEND_KI		"0x0C"
#define ID_RX_SET_KI		"0x0D"
#define ID_RX_SEND_KD		"0x0E"
#define ID_RX_SET_KD		"0x0F"

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


//extern void dyn_rcv_cb(uint8_t car);

unsigned char rx_data[RX_BUFFER_SIZE];
char ident_data[4];
char value_data[RX_BUFFER_SIZE-4];

void comm_Init(void)
{
	/* USART RECEPTION */
	HAL_USART_Receive_IT(&husart2, rx_data, RX_BUFFER_SIZE);
}


//====================================================================
//			utils
//====================================================================

void slice_str(const char * str, char * buffer, size_t start, size_t end)
{
    size_t j = 0;
    for ( size_t i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
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

void comm_SliceInput(const char * input, char * identifier, char *value)
{
	char tmp_value[RX_BUFFER_SIZE-4];
	char *tmp_slice;
	/* Slice input string from 0 to 3 == the identifier of shape 0xXX */
	slice_str(input, identifier, 0, 3);

	/* Slice input string from 5 to buffer size -1 == the value*/
	slice_str(input, tmp_value, 4, RX_BUFFER_SIZE); // rest of the string

	/* cast from char*[RX_BUFFER_SIZE-4], incompatible with strsep, to char * */
	tmp_slice = (char *)tmp_value;
	/* SEParate the string with ; as delimiter, keeps the string before ; */
	value = strsep(&tmp_slice, ";");
}

void comm_ComputeInput(char * identifier, char * value)
{
	/*
	 * str 1 == str 2 compare the address
	 * strcmp(str1,str2) compare the content
	 */

	if (strcmp(identifier,ID_RX_START) == 0)
	{
		//break; //temp
	}
	else if (strcmp(identifier,ID_RX_STOP) == 0)
	{
		//break; //temp
	}
	else if (strcmp(identifier,ID_RX_PWM_START) == 0)
	{
		pwm_Enable();
	}
	else if (strcmp(identifier,ID_RX_PWM_STOP) == 0)
	{
		pwm_Disable();
	}
	else if (strcmp(identifier,ID_RX_DUTY_SET) == 0)
	{
		/* Data sanitization done in SetVoltage */
		pwm_SetDuty((float)atof(value));
	}
	else if (strcmp(identifier,ID_RX_DIR) == 0)
	{
		int tmp_dir = atoi(value);
		/* data sanitization */
		if (tmp_dir < 0)
		{
			tmp_dir = 0;
		}
		else if (tmp_dir > 1)
		{
			tmp_dir = 1;
		}
		/* Change of direction */
		if (tmp_dir == 0)
		{
				mcc_SetCCW();
		}
		else
		{
				mcc_SetCW();
		}
	}
	else if (strcmp(identifier,ID_RX_VOLTAGE_SET) == 0)
	{
		/* Data sanitization done in SetVoltage */
		pwm_SetVoltage((float)atof(value));
	}
	else if (strcmp(identifier,ID_RX_SET_KP) == 0)
	{
		/* No data sanitization */
		pidFilter_SetKp((float)atof(value));
	}
	else if (strcmp(identifier,ID_RX_SET_KI) == 0)
	{
		/* No data sanitization */
		pidFilter_SetKi((float)atof(value));
	}
	else if (strcmp(identifier,ID_RX_SET_KD) == 0)
	{
		/* No data sanitization */
		pidFilter_SetKd((float)atof(value));
	}
	else
	{
		//break;
	}
}


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

	/* Decoupage données */
	comm_SliceInput((char *)rx_data, ident_data, value_data);

	/* Traitement données */
	comm_ComputeInput(ident_data, value_data);

	debugled_Toggle();
	printf("RECEIVED MESSAGE");

	/* Réarmement réception */
	//HAL_StatusTypeDef HAL_USART_Receive_IT(USART_HandleTypeDef *husart, uint8_t *pRxData, uint16_t Size)
	HAL_USART_Receive_IT(&husart2, rx_data, RX_BUFFER_SIZE);
}


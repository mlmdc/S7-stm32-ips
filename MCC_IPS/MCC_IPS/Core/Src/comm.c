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


//====================================================================
//			SIZE
//====================================================================

#define ID_CHAR_SIZE 4
#define FLOAT_CHAR_SIZE 16
#define INT_CHAR_SIZE 8
#define UINT32_CHAR_SIZE 16
#define UINT16_CHAR_SIZE 8


//====================================================================
//			RX IDENTIFIERS TABLE
//====================================================================
//ALL UNSIGNED CHAR
#define ID_RX_START			(unsigned char)0x01
#define ID_RX_STOP			(unsigned char)0x02
#define ID_RX_PWM_START		(unsigned char)0x03
#define ID_RX_PWM_STOP		(unsigned char)0x04
#define ID_RX_PWM_SET		(unsigned char)0x05
#define ID_RX_DIR			(unsigned char)0x06

//====================================================================
//			TX IDENTIFIERS TABLE
//====================================================================
//ALL UNSIGNED CHAR
//ID size = 4 chars
#define ID_TX_ADC			(unsigned int)0x10
#define ID_TX_PWM			(unsigned int)0x11
#define ID_TX_RPM			(unsigned int)0x12
#define ID_TX_DIR			(unsigned int)0x13


uint8_t rec_buf[2];
uint8_t rec_buf6[2]="";
int put_char(int);

extern void dyn_rcv_cb(uint8_t car);



void comm_SendData(void)
{
	//DATA_FORMAT : tx_data[x] = [ID_TX_TypeOfData]+[TypeOfData_0]+...+[TypeOfData_n]+[ID_TX_TypeOfData...]+...

	//printf("\n\r");
	/* ADC */
	printf("0x%x, ", ID_TX_ADC); //hex id
	printf("%f; ", ADC_LastValue);
	/* PWM */
	printf("0x%x, ", ID_TX_PWM); //hex id
	printf("%u; ", (unsigned int)PWM_Value); //uchar typecast for warning
	/* PWM */
	printf("0x%x, ", ID_TX_RPM); //hex id
	printf("%f; ", output_speed);

	printf("\n\r");
}


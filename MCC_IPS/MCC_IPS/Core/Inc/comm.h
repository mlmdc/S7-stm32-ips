/*
 * comm.h
 *
 *  Created on: Mar 24, 2024
 *      Author: Madec
 */


#ifndef INC_COMM_H_
#define INC_COMM_H_

#include "main.h"

void comm_Init(void);

void debug_readRx(void);

void comm_SendData(void);

void comm_SliceInput(const char * input, char * identifier, char *value);

void comm_ComputeInput(char * identifier, char * value);

void HAL_USART_RxCpltCallback(UART_HandleTypeDef *huart);


#endif /* INC_COMM_H_ */

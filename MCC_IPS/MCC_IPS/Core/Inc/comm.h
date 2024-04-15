/*
 * comm.h
 *
 *  Created on: Mar 24, 2024
 *      Author: amay
 */


#ifndef INC_COMM_H_
#define INC_COMM_H_

#include "main.h"

void comm_init(void);

void comm_SendData(void);

void comm_sliceInput(const char * input, char * identifier, char *value);

void comm_computeInput(char * identifier, char * value);

void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart);


#endif /* INC_COMM_H_ */

/*
 * speed.h
 *
 *  Created on: Mar 20, 2024
 *      Author: amay
 */

#ifndef SRC_SPEED_H_
#define SRC_SPEED_H_

#include "main.h"

//TIM_HandleTypeDef TimEncoderHandle;

void encoder_Init(void);

void encoder_updatePosition(int * AnglePosition);

uint16_t encoder_GetPos16(void);

uint16_t encoder_GetNTurn16(void);

uint32_t encoder_GetNandPos(void);

int16_t encoder_GetSpeed(void);

void encoder_CallbackIndex();

#endif /* SRC_SPEED_H_ */

/*
 * speed.h
 *
 *  Created on: Mar 20, 2024
 *      Author: madec
 */

#ifndef INC_SPEED_H_
#define INC_SPEED_H_

#include "main.h"

//TIM_HandleTypeDef TimEncoderHandle;

extern volatile float output_speed;

void encoder_Init(void);

uint16_t encoder_FetchPosition();
void encoder_UpdatePosition();

void encoder_GetSpeed();

void encoder_SpeedUpdate(float* source, float new_speed);

float encoder_EWMAverage(float* source, float alpha);

void encoder_CallbackIndex();

#endif /* INC_SPEED_H_ */

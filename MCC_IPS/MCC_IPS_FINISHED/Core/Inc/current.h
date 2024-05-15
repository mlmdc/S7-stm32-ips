/*
 * accelerometer.h
 *
 *  Created on: Apr 9, 2024
 *      Author: amay
 */

#ifndef INC_CURRENT_H_
#define INC_CURRENT_H_

#include "main.h"

extern volatile float current_LastVoltage;


void current_Init(void);

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

void current_ReadDataBlocking(uint32_t* destination);

void current_int2flt(uint32_t* source, float* destination);

//For best accuracy
// The higher the value of alpha, the more closely the EWMA tracks the original time series
float current_EWMAverage(float* source, float alpha);


void current_GetValue(void);



#endif /* INC_CURRENT_H_ */

/*
 * current.h
 *
 *  Created on: Mar 20, 2024
 *      Author: amay
 */

#ifndef INC_CURRENT_H_
#define INC_CURRENT_H_

#include "main.h"

extern volatile float ADC_LastValue;

void current_Init(void);

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

void adc_ReadDataBlocking(uint32_t* destination);

void adc_int2flt(uint32_t* source, float* destination);

void adc_EWMAverage(float* source, float alpha);

void adc_GetValue();

#endif /* INC_CURRENT_H_ */

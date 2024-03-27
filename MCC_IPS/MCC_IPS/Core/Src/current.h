/*
 * current.h
 *
 *  Created on: Mar 20, 2024
 *      Author: amay
 */

#ifndef SRC_CURRENT_H_
#define SRC_CURRENT_H_

#include "main.h"

extern volatile float ADC_LastValue;

//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

void adc_ReadData(int32_t* destination, int n_conv);

void adc_int2flt(int32_t* source, float* destination);

void adc_EWMAverage(float* source, float destination, int alpha);

void adc_GetValue();

#endif /* SRC_CURRENT_H_ */

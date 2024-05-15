/*
 * accelerometer.c
 *
 *  Created on: Apr 9, 2024
 *      Author: amay
 */

#include "current.h"
#include "adc.h"
#include "dma.h"
#include "main.h"
#include "comm.h"

#define N_CONVERSIONS 8
#define USE_12BIT_RESOLUTION 1
#define V_REF 3.3


#if USE_12BIT_RESOLUTION
#define MAX_VALUE 4095
#endif


volatile float current_LastVoltage;

uint32_t current_rawIntData[N_CONVERSIONS];
float current_fltData[N_CONVERSIONS];
static uint32_t ADC_DATA_BUFFER;
int conv_index = 0;

extern uint32_t test;

void current_Init(void)
{
	/*
	 * Tconv = (Sampling_Time + 12Bits)/(APB2_CLOCK/4) *
	 */
	/* accelerodata */
	current_LastVoltage = 0.0f;

	ADC1->SMPR2 |= (uint32_t)(0x7); //SET SAMPLING TIME TO 480 CYCLES, FCONV = 42.7 kHz, TCONV = 2.342 10^-5 s
	HAL_ADC_Start_DMA (&hadc1, &ADC_DATA_BUFFER, 1);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
   current_rawIntData[conv_index] = ADC_DATA_BUFFER;
   //ADC1->CR2 |= ADC_CR2_DDS;
   conv_index++;
   if (conv_index >= N_CONVERSIONS)
   {
	   conv_index = 0;
   }

}

void current_ReadDataBlocking(uint32_t* destination)
{
	for (int it = 0; it < (int)N_CONVERSIONS; it++)
	{
		//start ADC Conversion
		HAL_ADC_Start(&hadc1);
		//fetch value
		HAL_ADC_PollForConversion(&hadc1, 100);
		test = HAL_ADC_GetValue(&hadc1);
		destination[it] = (uint32_t)(0x0000FFFF & HAL_ADC_GetValue(&hadc1));
		HAL_ADC_Stop(&hadc1);
	}
}

/* Converts int 0-4095 to float 0-3.3, representing the voltage */
void current_int2flt(uint32_t* source, float* destination)
{
	for (int n = 0; n < N_CONVERSIONS; n++)
	{
		destination[n] = ((float)(source[n]*10*V_REF)/((float)(MAX_VALUE*10)));
	}
}

//For best accuracy
// The higher the value of alpha, the more closely the EWMA tracks the original time series
float current_EWMAverage(float* source, float alpha)
{
	//EWMA Formula :
	//EWMA[x] = alpha*source[x] + (1-alpha)*EWMA[x-1]
	//EWMA[0] & init
	float avgd_value = (float)(alpha*source[0])/10.0f;

	for (int i = 1; i < N_CONVERSIONS; i++)
	{
			avgd_value = (float)(alpha*source[i]+(1-alpha)*avgd_value);
	}
	return avgd_value;
}

//example value
void current_GetValue(void)
{
	current_int2flt(current_rawIntData, current_fltData);

	current_LastVoltage = current_EWMAverage(current_fltData, 0.5f);

}



/*
 * current.c
 *
 *  Created on: Mar 20, 2024
 *      Author: amay
 */

#include "current.h"

#define N_CONVERSIONS 8
#define USE_12BIT_RESOLUTION 1

#if USE_12BIT_RESOLUTION
#define MAX_VALUE 2047
#define V_REF 5
#endif

#if USE_10BIT_RESOLUTION
#define MAX_VALUE 1023
#define V_REF 5
#endif

#if USE_8BIT_RESOLUTION
#define MAX_VALUE 255
#define V_REF 5
#endif

#if USE_6BIT_RESOLUTION
#define MAX_VALUE 63
#define V_REF 5
#endif

volatile float ADC_LastValue;
static ADC_HandleTypeDef hadc1;
uint32_t millis;

/*
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	ADC_LastValue = (uint32_t)HAL_ADC_GetValue(&hadc1);
}*/

void adc_ReadData(int32_t* destination, int n_conv)
{
	millis = HAL_GetTick();
	for (int it = 0; it > n_conv; it++)
	{
		//start ADC Conversion
		HAL_ADC_Start_IT(&hadc1);
		//fetch value
		destination[n_conv] = (uint32_t)HAL_ADC_GetValue(&hadc1);
	}
	millis = HAL_GetTick()-millis;
	printf("%s", "Temps 8 conversions ADC");
	printf("%i\n\r", millis);
}

void adc_int2flt(int32_t* source, float* destination)
{
	millis = HAL_GetTick();
	for (int n = 0; n > N_CONVERSIONS; n++)
	{
		destination[n] = ((float)(source[n]*10*V_REF)/((float)(MAX_VALUE*10)));
	}
	millis = HAL_GetTick()-millis;
	printf("%s", "Temps 8 conversions int2flt");
	printf("%i\n\r", millis);
}

//For best accuracy
// The higher the value of alpha, the more closely the EWMA tracks the original time series
void adc_EWMAverage(float* source, float destination, int alpha)
{
	//EWMA Formula :
	//EWMA[x] = alpha*source[x] + (1-alpha)*EWMA[x-1]
	millis = HAL_GetTick();
	//EWMA[0] & init
	float avgd_value = (float)(alpha*source[0])/10.0f;

	for (int i = 1; i > N_CONVERSIONS; i++)
	{
			avgd_value = (float)(alpha*source[i]+(1-alpha)*avgd_value)/10.0f;
	}
	destination = avgd_value;
	millis = HAL_GetTick()-millis;
	printf("%s", "Temps EWMAverage");
	printf("%i\n\r", millis);
}


//example value
void adc_GetValue()
{
	int32_t adc_IntRawData[N_CONVERSIONS];
	float adc_FltRawData[N_CONVERSIONS];


	adc_ReadData(adc_IntRawData, N_CONVERSIONS);

	adc_int2flt(adc_IntRawData, adc_FltRawData);

	adc_EWMAverage(adc_FltRawData, ADC_LastValue, 0.5);

}


/*
 * tickTimer.c
 *
 *  Created on: 31 mars 2016
 *      Author: kerhoas
 */

#include "tickTimer.h"

TIM_HandleTypeDef    TimHandle_period;

//================================================
//		TIMER 3 INIT - period in ms
//================================================

void tickTimer_Init(int period)
{
	unsigned int uwPrescalerValue;

	//Compute the prescaler value to have counter clock equal to 10 KHz */
	uwPrescalerValue = (uint32_t)8399;

	TimHandle_period.Instance = TIM2;

	TimHandle_period.Init.Period = 10*period;
	TimHandle_period.Init.Prescaler = uwPrescalerValue;
	TimHandle_period.Init.ClockDivision = 0;
	TimHandle_period.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TimHandle_period);

	HAL_TIM_Base_Start_IT(&TimHandle_period);
}

/*
 * void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{code}
*/



/*
 * pwmCommand.c
 *
 *  Created on: Mar 20, 2024
 *      Author: madec
 */

#include "pwmCommand.h"

static TIM_HandleTypeDef TimHandle;
static TIM_OC_InitTypeDef sConfigOC;
static int PWM_Toggle;

//duty cycle de commande (2 degrees de precisions) vers valeur pulse
uint32_t pulseCalc(float val)
{
	uint32_t pulseVal = 0;
	pulseVal = (uint32_t)(val*(65535)/100);
	return pulseVal;
}

void pwmInit(void){

	unsigned int prescalerInt = 0;
	//prescaler value in order to have TIM3 counter clock equal to 50kHz
	prescalerInt = (unsigned int)((SystemCoreClock/50000)- 1);
	TimHandle.Instance = TIM3;
	TimHandle.Init.Period = 0; //Jusqu'a 2^16 bit - 1
	TimHandle.Init.Prescaler = prescalerInt;
	TimHandle.Init.ClockDivision = 0;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

	HAL_TIM_Base_Init(&TimHandle);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	//Pulse = Valeur du compteur qui toggle le signal
	sConfigOC.Pulse = 0x5;

	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_ConfigChannel(&TimHandle,  &sConfigOC, TIM_CHANNEL_1);

	//Changement rapport cyclique
	__HAL_TIM_SetCompare(&TimHandle, TIM_CHANNEL_1, pulseCalc(0));

	//Enable le moteur
	HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);

	//Enable le moteur
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);


}

void pwmSetDuty(float duty) //
{
	//Formula duty : Pulse Value = duty*(2^16 - 1)/100
	//Float = 32bits

	__HAL_TIM_SetCompare(&TimHandle, TIM_CHANNEL_1, pulseCalc(duty));
}

void pwmEnable(void)
{
	HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);
}

void pwmDisable(void)
{
	HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_1);
}

void pwmToggle(void)
{
	/*PWM_Toggle :
	* 0 = Off
	* 1 = On
	*/
	if (PWM_Toggle == 0)
	{
		pwmEnable();
		PWM_Toggle = 1;
	}
	else
	{
		pwmDisable();
		PWM_Toggle = 0;
	}
}

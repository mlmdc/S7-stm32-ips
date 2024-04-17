/*
 * mccCommand.c
 *
 *  Created on: Mar 25, 2024
 *      Author: Madec
 */

#include "main.h"

// PB6 : CW/CCW
// PC8 : BRAKE
// PC9 :
static int isCW = 0;
static int isLedOn = 0;

/*
 * CW : DIR = 1
 *
 * CCW : DIR = 0
 *
 */
void debugled_SetOn(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
}

void debugled_SetOff(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
}

void debugled_Toggle(void)
{
	if (isCW == 0)
	{
			debugled_SetOn();
			isLedOn = 1;
	}
	else
	{
			debugled_SetOff();
			isLedOn = 0;
	}
}

void mcc_SetCW(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
	isCW = 1;
}

void mcc_SetCCW(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);
	isCW = 0;
}

void mcc_SetBraking(int state)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, (state%2));
}

void mcc_ToggleDir(void)
{
	if (isCW == 0)
	{
		mcc_SetCCW();
		isCW = 1;
	}
	else
	{
		mcc_SetCW();
		isCW = 0;
	}

}

/*
 * mccCommand.c
 *
 *  Created on: Mar 25, 2024
 *      Author: amay
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
void debugled_setOn(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
}

void debugled_setOff(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
}

void debugled_toggle(void)
{
	if (isCW == 0)
	{
			debugled_setOn();
			isLedOn = 1;
	}
	else
	{
			debugled_setOff();
			isLedOn = 0;
	}
}

void mcc_setCW(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
	isCW = 1;
}

void mcc_setCCW(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);
	isCW = 0;
}

void mcc_setBraking(int state)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, (state%2));
}

void mcc_toggleDir(void)
{
	if (isCW == 0)
	{
		mcc_setCCW();
		isCW = 1;
	}
	else
	{
		mcc_setCW();
		isCW = 0;
	}

}

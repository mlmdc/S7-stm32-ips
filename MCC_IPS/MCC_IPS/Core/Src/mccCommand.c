/*
 * mccCommand.c
 *
 *  Created on: Mar 25, 2024
 *      Author: amay
 */

#import "main.h"

// PB6 : CW/CCW
// PC8 : BRAKE
// PC9 :
static int isCW = 0;

void mcc_setCW(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);
}

void mcc_setCCW(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
}

void mcc_setBraking(int state)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, (state%2));
}

void mcc_toggleDir()
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

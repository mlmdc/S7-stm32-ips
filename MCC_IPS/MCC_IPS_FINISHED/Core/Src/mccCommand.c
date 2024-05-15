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

/*
 * CW : DIR = 1
 *
 * CCW : DIR = 0
 *
 */


void mcc_SetCW(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6, GPIO_PIN_SET);
	isCW = 1;
}

void mcc_SetCCW(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6, GPIO_PIN_RESET);
	isCW = 0;
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

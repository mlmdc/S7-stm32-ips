/*
 * speed.c
 *
 *  Created on: Mar 20, 2024
 *      Author: Madec
 */

#define TE_ms 5 //
#define COUNT_PER_ROUND 1000
#define MAX_CNT_PER_REV ((COUNT_PER_ROUND)*4 - 1)
#define MAX_COUNT (int)(((unsigned long)MAX_CNT_PER_REV*8192)/1000)
#define HALF_MAX_COUNT (MAX_COUNT >> 1)
#define COEFF 8192

#include "speed.h"

TIM_HandleTypeDef TimEncoderHandle;

static int index = 0;

void encoder_Init(void){

	TIM_Encoder_InitTypeDef sConfig;

	TimEncoderHandle.Instance = TIM1;
	TimEncoderHandle.Init.Prescaler = 0;
	TimEncoderHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimEncoderHandle.Init.Period = COUNT_PER_ROUND*4;
	TimEncoderHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	//TIM1 Channel1 Mode
	sConfig.IC1Polarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV4;
	sConfig.IC1Filter = 0x0F;
	//TIM1 Channel2 Mode
	sConfig.IC2Polarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV4;
	sConfig.IC2Filter = 0x0F;

	HAL_TIM_Encoder_Init(&TimEncoderHandle, &sConfig);
	HAL_TIM_Encoder_Start(&TimEncoderHandle, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&TimEncoderHandle, TIM_CHANNEL_2);

}


void encoder_updatePosition(int * AnglePosition)
{
	int TIMCNT = TIM1->CNT;
	AnglePosition[1] = AnglePosition[0];
	//AnglePosition[0] = int
	AnglePosition[0] = (unsigned int)(((unsigned long)TIMCNT * COEFF)/1000);
}

uint16_t encoder_GetPos16(void)
{
	int TIMCNT = TIM1->CNT;
	return (uint16_t)(TIMCNT);
}

uint16_t encoder_GetNTurn16(void)
{
	uint16_t pos = 0;
	pos = index*4*COUNT_PER_ROUND;
	return pos;
}

uint32_t encoder_GetNandPos(void)
{
	uint32_t NPos = 0;
	NPos = (uint32_t)(encoder_GetPos16() + encoder_GetNTurn16());
	return NPos;
}

int16_t encoder_GetSpeed(void)
{
	static int AngPos[2] = {0,0};
	static int16_t Speed = 0;
	int Speed_rpm;

	encoder_updatePosition(AngPos);
	//Speed mais n'est pas mathématiquement une vitesse, utilisation en prétraitement
	Speed = AngPos[0]- AngPos[1];
	if (Speed >= 0)
	{
		if (Speed >= HALF_MAX_COUNT)
		{
			Speed = Speed - MAX_COUNT;
		}
	}
	else
	{
		if (Speed <= HALF_MAX_COUNT)
		{
			Speed = Speed + MAX_COUNT;
		}
	}

	// Conversion en RPM
	// 1 Tour = 32767
	// Nbre tours pendant Te : DELTA DE POS/32767
	// Nbre tours pendant 1s (soit 1/Te fois) : (DELTA DE POS/32767)*(1000/Te)
	// Nbre de tours min : La meme chose x60

	Speed_rpm = (Speed*60*1000)*(32767*TE_ms);

	return Speed_rpm;

}


void encoder_CallbackIndex()
{
	if (__HAL_TIM_DIRECTION_STATUS(&TimEncoderHandle)==1)
	{
		index--;
	}
	else
	{
		index++;
	}
	__HAL_TIM_SetCounter(&TimEncoderHandle, 0);
	HAL_TIM_Encoder_Start(&TimEncoderHandle, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&TimEncoderHandle, TIM_CHANNEL_2);

}


//(uint32_t)(float*1000) // 3340.91 = 3340
		// nombre 0xF000, 0x0f00,





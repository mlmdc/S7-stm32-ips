/*
 * filter.c
 *
 *  Created on: Apr 7, 2024
 *      Author: amay
 */

#include "pidFilter.h"
#include "main.h"
#include "speed.h"
#include "pwmCommand.h"
#include <math.h>


//====================================================================
//			PID
//====================================================================

volatile float Kp;
volatile float Kd;
volatile float Ki;
/*volatile*/ uint32_t te_ms;
volatile float Oe_k[3] = {0,0,0};
volatile float Uc_k[2] = {0,0};
volatile float command;

float Ti = 0;
float Td = 0;
float k_alpha = 0;

float b2;
float b1;
float b0;
float a2;
float a1;


void pidFilter_param_init(void)
{
	/* PID values */
	Kp = 0.01f;
	Ki = 0.0006666666666666666f;
	Kd = 0.09;
	te_ms = 70;
	Ti = (Kp/Ki)*(float)te_ms;
	Td = (Kd/Kp)*(float)te_ms;
	command = 0.0f;

	/* PID coefficients init */

	b2 = Kp+Kd+Ki;
	b1 = -2*Kd-Kp;
	b0 = Kd;
	a2 = 1;
	a1 = -1;
}

void pidFilter_setKp(float new_Kp)
{
	Kp = new_Kp;
}

void pidFilter_setKi(float new_Ki)
{
	Ki = new_Ki;
}

void pidFilter_setKd(float new_Kd)
{
	Kd = new_Kd;
}

void pidFilter_setCommand(float new_command)
{
	command = new_command;
}

void pidFilter_setCommandVoltage(float new_voltage)
{
	float new_command = 3000*(new_voltage/24);
	pidFilter_setCommand(new_command);
}


void pidFilter_compute(void)
{
	/* set k-1 */
	Oe_k[2] = Oe_k[1];
	Oe_k[1] = Oe_k[0];
	Oe_k[0] = command - (float)fabs(output_speed);

	Uc_k[1] = Uc_k[0];

	/* calculate Uc(z) */
	Uc_k[0] = (b2/a2)*Oe_k[0] + (b1/a2)*Oe_k[1] + (b0/a2)*Oe_k[2] - (a1/a2)*Uc_k[1];

	/* no negative voltage */
	if (Uc_k[0] < 0.0f)
	{
		Uc_k[0] = 0.0f;
	}
	else if (Uc_k[0] > 24.0f) /* no voltage superior to 24, both as a command & status*/
	{
		Uc_k[0] = 24.0f;
	}
	/* PWM new duty */
	//pwm_SetVoltage(Uc_k[0]);
}

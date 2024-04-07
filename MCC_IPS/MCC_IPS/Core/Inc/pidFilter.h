/*
 * filter.h
 *
 *  Created on: Apr 7, 2024
 *      Author: amay
 */

#ifndef INC_PIDFILTER_H_
#define INC_PIDFILTER_H_

#include "main.h"
//====================================================================
//			PID
//====================================================================

extern volatile float Kp;
extern volatile float Kd;
extern volatile float Ki;
//extern volatile uint32_t te_ms;
extern volatile float Oe_k[3];
extern volatile float Uc_k[2];
extern volatile float command;

void pidFilter_param_init(void);

void pidFilter_setKp(float new_Kp);

void pidFilter_setKi(float new_Ki);

void pidFilter_setKd(float new_Kd);

void pidFilter_compute(void);

#endif /* INC_PIDFILTER_H_ */

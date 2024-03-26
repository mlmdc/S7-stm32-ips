/*
 * pwmCommand.h
 *
 *  Created on: Mar 20, 2024
 *      Author: madec
 */

#ifndef SRC_PWMCOMMAND_H_
#define SRC_PWMCOMMAND_H_

#include "main.h"

uint32_t pulseCalc(float val);

void pwmInit(void);

void pwmSetDuty(float duty);

void pwmEnable(void);

void pwmDisable(void);

void pwmToggle(void);

/*fonction d'informations
 *
 * void getPwm(void);
 * void getDuty(void);
 */

#endif /* SRC_PWMCOMMAND_H_ */

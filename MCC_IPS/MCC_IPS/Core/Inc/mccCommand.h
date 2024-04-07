/*
 * mccCommand.h
 *
 *  Created on: Mar 25, 2024
 *      Author: amay
 */


#ifndef INC_MCCCOMMAND_H_
#define INC_MCCCOMMAND_H_

#include "main.h"

void mcc_setCW(void);

void mcc_setCCW(void);

void mcc_setBraking(int state);

void mcc_toggleDir(void);

#endif /* INC_MCCCOMMAND_H_ */

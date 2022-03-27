/*
 * system.h
 *
 * system Header File
 *
 *  Created on: 22/05/2020
 *      Author: Jason Ui
 */

#include <stdlib.h>
#include <stdint.h>

#ifndef SYSTEM_H_
#define SYSTEM_H_

void initClock (void);
void initDisplayTimerInt(void (*pfnHandler) (void));
void DisplayTimerIntHandler(void);
void DisplayTimerIntClear(void);
void DisableDisplayTimer(void);

//variables
int16_t stepCount;
int16_t distancetravelled;
int16_t distStp; //a different step count used for measuring distance
int16_t acc_vector;

#endif /* SYSTEM_H_ */

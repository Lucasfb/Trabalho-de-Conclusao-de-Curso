/*
 * setup.h
 *
 *  Created on: 25 de set de 2018
 *      Author: lucas
 */

#ifndef SETUP_H_
#define SETUP_H_

#include "F28x_Project.h"


#define CPU_FRQ_200MHZ          1


void FPU_initSystemClocks(void);
void FPU_initEpie(void);
void FPU_initFlash(void);
//extern void InitFlash_PrefetchCacheDisable(void);
//extern void InitFlash_PrefetchCacheEnable(void);
//void SetupFlash(unsigned short waitStates);
void done(void);

#endif /* SETUP_H_ */

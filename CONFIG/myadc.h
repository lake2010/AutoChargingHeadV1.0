#ifndef _MYADC_H 
#define _MYADC_H

#include "stm32f10x_adc.h"

void  CMoveBase_CurrentSenseInit(void);
u16 CMoveBase_CurrentSenseOnce(u8 ch);
u16 CMoveBase_CurrentSense(u8 ch,u8 times);
#endif


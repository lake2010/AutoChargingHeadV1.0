#ifndef _SYSSTATUS_H
#define _SYSSTATUS_H

#include "stm32f10x.h"
typedef struct SYSstatus_parameter_all
{
	char T_V_sensor;     //接触标志
	float A;			 //电流值
	//char CurrentFlag; 	 //电流标志
	char VoltageFlag; 	 //电压标志
	char ChargeFlag;//开始充电
}SYSstatus_parameter;


#endif


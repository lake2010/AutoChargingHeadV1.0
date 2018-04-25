#include "ChargeBase.h"
#include "config.h"
#include "commen.h"
#include "usart.h"
#include "delay.h"
#include "myadc.h"
#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
CChargeBase_parameter ChargeBase;//充电头状态量总定义

char CChargeBase_JudgePortStatus(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	char flag;
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0)
			flag = 0;
	}
	else if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 1)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 1)
			flag = 1;
	}
	return flag;
}


void AutoCharging_loop(CChargeBase_parameter *CChargeBase_para)
{
	CChargeBase_para->m_SYSstatus.T_V_sensor = CChargeBase_JudgePortStatus(m_T_VSensor_Port.c_gpio,m_T_VSensor_Port.c_pin);
	if(!CChargeBase_para->m_SYSstatus.T_V_sensor)
	{
		GPIO_WriteBit(m_ToucherSensor_Port.c_gpio,m_ToucherSensor_Port.c_pin,Bit_RESET);
		GPIO_WriteBit(m_Relay_Port.c_gpio,m_Relay_Port.c_pin,Bit_RESET);
		CChargeBase_para->m_SYSstatus.ChargeFlag  = 0;		
	}
	else
	{	
		if(CChargeBase_para->m_SYSstatus.ChargeFlag == 0)
		{
			GPIO_WriteBit(m_ToucherSensor_Port.c_gpio,m_ToucherSensor_Port.c_pin,Bit_SET);
			delay_ms(1000);
			GPIO_WriteBit(m_Relay_Port.c_gpio,m_Relay_Port.c_pin,Bit_SET);
			delay_ms(1500);
			CChargeBase_para->m_SYSstatus.ChargeFlag  = 1;
		}
		else
		{
			CChargeBase_para->m_SYSstatus.A = CMoveBase_CurrentSense(ADC_Channel_3,10)*3.3/4095 ;
			CChargeBase_para->m_SYSstatus.A = (CChargeBase_para->m_SYSstatus.A - 2.5)/0.066;
			if(CChargeBase_para->m_SYSstatus.A >= 1.5)
			{
				CChargeBase_para->m_LED.m_chargeLedBlink = true;
				GPIO_WriteBit(m_CurrentFlag_Port.c_gpio,m_CurrentFlag_Port.c_pin,Bit_SET);
			}
			else
			{
				GPIO_WriteBit(m_ToucherSensor_Port.c_gpio,m_ToucherSensor_Port.c_pin,Bit_RESET);
				GPIO_WriteBit(m_Relay_Port.c_gpio,m_Relay_Port.c_pin,Bit_RESET);
				GPIO_WriteBit(m_CurrentFlag_Port.c_gpio,m_CurrentFlag_Port.c_pin,Bit_RESET); 
				delay_ms(1500);
			}		
		}
	}
}

void AutoCharging_setup(CChargeBase_parameter *CChargeBase_para)
{
	delay_init();
	ChargeBasePinDefine();
	ConfigClass_USART1Init(UART1_BAUD);
	CMoveBase_CurrentSenseInit();	//PA1引脚作为AD输入
	CLED_setup( &CChargeBase_para->m_LED, &m_CurrentLed_Port );
	CChargeBase_IWDGInit(4,3125);//与分频数为64,重载值为625,溢出时间为1s	
}



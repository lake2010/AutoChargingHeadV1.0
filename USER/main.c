#include "stm32f10x.h"
#include "config.h"
#include "ChargeBase.h"
#include "delay.h"
#include "ChargeBase.h"
 int main(void)
 {	  
	 MySystem_init();
	 AutoCharging_setup(&ChargeBase);
	 while(1)
	 {
		 AutoCharging_loop(&ChargeBase);
		 CChargeBaseLED_loop(&ChargeBase.m_LED);
		 CChargeBaseIWDGFeed_loop(); 
	 }
 }
 
 

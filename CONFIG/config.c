#include "config.h"

//外设 m--》machien
Gpio_pin_parameter m_CurrentLed_Port; 
Gpio_pin_parameter m_Relay_Port;
Gpio_pin_parameter m_MotionRelay_Port;
Gpio_pin_parameter m_CurrentFlag_Port;
Gpio_pin_parameter m_ToucherSensor_Port;
Gpio_pin_parameter m_T_VSensor_Port;
//外设引脚设置定义
void ChargeBasePinDefine(void)
{
	//RUN
	m_CurrentLed_Port.c_gpio = GPIOB;
	m_CurrentLed_Port.c_pin = GPIO_Pin_9;
	//RELAY2
	m_Relay_Port.c_gpio = GPIOB;
	m_Relay_Port.c_pin = GPIO_Pin_8;
	//RELAY1
	m_MotionRelay_Port.c_gpio = GPIOA;
	m_MotionRelay_Port.c_pin = GPIO_Pin_8;
	//CurrentFlag
	m_CurrentFlag_Port.c_gpio = GPIOC;
	m_CurrentFlag_Port.c_pin = GPIO_Pin_13;
	//Toucher_sensor
	m_ToucherSensor_Port.c_gpio = GPIOB;
	m_ToucherSensor_Port.c_pin = GPIO_Pin_5;
	//T_V_sensor
	m_T_VSensor_Port.c_gpio = GPIOA;
	m_T_VSensor_Port.c_pin = GPIO_Pin_1;
	
	pinOutputModeInit(&m_ToucherSensor_Port);
	pinOutputModeInit(&m_Relay_Port); 
	pinOutputModeInit(&m_CurrentLed_Port);
	pinOutputModeInit(&m_CurrentFlag_Port);
	//上拉输入，低电平有效
	pinInputModeInit(&m_T_VSensor_Port);
	//运动板控制器信号初始化
	pinInputModeInit(&m_MotionRelay_Port);
	GPIO_WriteBit(m_ToucherSensor_Port.c_gpio,m_ToucherSensor_Port.c_pin,Bit_RESET);
}


//外设时钟使能
void pinInputModeInit(Gpio_pin_parameter* Gpio_pin_para)
{
	GPIO_InitTypeDef GPIO_InitStructrue;
	if(Gpio_pin_para->c_gpio == GPIOA)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOB)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOC)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOD)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOE)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOE,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOF)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOF,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOG)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOG,ENABLE);
	
	GPIO_InitStructrue.GPIO_Pin = Gpio_pin_para->c_pin;
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_IPU;  			//上拉输入 所以低电平有效
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_50MHz;		//输出模式最大速度
	GPIO_Init(Gpio_pin_para->c_gpio,&GPIO_InitStructrue);   //g根据设定参数配置GPIO
}



//外设时钟使能
void pinOutputModeInit(Gpio_pin_parameter* Gpio_pin_para)
{
	GPIO_InitTypeDef GPIO_InitStructrue;
	if(Gpio_pin_para->c_gpio == GPIOA)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOB)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOC)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOD)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOE)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOE,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOF)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOF,ENABLE);
	else if(Gpio_pin_para->c_gpio == GPIOG)
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOG,ENABLE);

	GPIO_InitStructrue.GPIO_Pin = Gpio_pin_para->c_pin;
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP; //通用推挽输出模式
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_50MHz;//输出模式最大速度
	GPIO_Init(Gpio_pin_para->c_gpio,&GPIO_InitStructrue);//g根据设定参数配置GPIO
}

void RCC_SystemClkInit(void)
{
	RCC_DeInit();             
	//RCC_HSEConfig(RCC_HSE_Bypass);
	RCC_HSEConfig(RCC_HSE_ON);   
	while(RCC_WaitForHSEStartUp() != SUCCESS);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); 
	FLASH_SetLatency(FLASH_Latency_2);
	RCC_HCLKConfig(RCC_SYSCLK_Div1); //HCLK = SYSCLK 
	RCC_PCLK2Config(RCC_HCLK_Div1);  //PCLK2 = HCLK
	RCC_PCLK1Config(RCC_HCLK_Div2);  //PCLK1 = HCLK/2 
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);//PLLCLK = 8MHz * 9 = 72 MHz 
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
	RCC_HSICmd(DISABLE);
}

void pinDigitalWrite( Gpio_pin_parameter* Gpio_pin_para, boolean fal )
{
	if( fal == false )
		GPIO_ResetBits(Gpio_pin_para->c_gpio,Gpio_pin_para->c_pin);
	else if( fal == true )
		GPIO_SetBits(Gpio_pin_para->c_gpio,Gpio_pin_para->c_pin);
	else
		GPIO_ResetBits(Gpio_pin_para->c_gpio,Gpio_pin_para->c_pin);
}

uint8_t pinDigitalRead( Gpio_pin_parameter* Gpio_pin_para )
{
	return GPIO_ReadInputDataBit( Gpio_pin_para->c_gpio, Gpio_pin_para->c_pin );
}
//系统时钟设置 SYSCLK AHB APB2 PLL为72M APB1为36M
void MySystem_init(void)
{
	RCC_SystemClkInit();	//系统时钟设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}


#include "speed.h"
#include "oled.h"
#include "usart.h"

extern int CountSensor_Count;				//全局变量，用于计数
extern float CarSpeed;
extern int CountSensor_Count1;
int _1s_number = 0;      //定时器中断次数的计数值，100次即为1s
float Diameter = 6.5; //小车车轮的直径
float pi = 3.14;

void speed_init()
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//开启AFIO的时钟，外部中断必须开启AFIO的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA5引脚初始化为上拉输入
	
	/*AFIO选择中断引脚*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);//将外部中断的5号线映射到GPIOB，即选择PA5为外部中断引脚
	
	/*EXTI初始化*/
	EXTI_InitTypeDef EXTI_InitStructure;						//定义结构体变量
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;					//选择配置外部中断的14号线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//指定外部中断线使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//指定外部中断线为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//指定外部中断线为下降沿触发 EXTI_Trigger_Falling
	EXTI_Init(&EXTI_InitStructure);								//将结构体变量交给EXTI_Init，配置EXTI外设
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;		//选择配置NVIC的EXTI9_5线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//指定NVIC线路的抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//指定NVIC线路的响应优先级为0
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}
uint16_t CountSensor1_Get(void)
{
	return CountSensor_Count1;
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) == SET)		//判断是否是外部中断5号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)
		{
			CountSensor_Count ++;					//计数值自增一次
			CountSensor_Count1 ++;
		}
		EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE5上的中断标志位  
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}
// 定时器中断函数，可以复制到使用它的地方 1s
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		CountSensor_Count = CountSensor_Get();
		CarSpeed=((CountSensor_Count/20)*pi*Diameter);
		CountSensor_Count=0;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
//void TIM4_IRQHandler(void)
//{
//if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
//{
//_1s_number++;
//if(_1s_number==100)
//{	
// 
//CarSpeed = ((CountSensor_Count/20.0000)*pi*Diameter)/1000.0000;
//CountSensor_Count=0;
//_1s_number=0;
//}
// 
//TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
//}
// 
//}



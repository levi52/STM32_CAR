#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "timer.h"
#include "speed.h"
#include "oled.h"
#include "adc.h"
#include "tcrt.h"
#include "length.h"

/**
************************************************************
* @file         
* @brief     
* @author    levi5.
* @date  	 2024/1/15
* @version   v5
* @note   
***********************************************************/


int g_USART1_FLAG =0;
int g_USART3_FLAG =0;

//int i=0;
int CountSensor_Count;
int CountSensor_Count1;
float CarSpeed;
u8 string[10] = {0};	//OLED字符串显示
int t1;
int t2;
float t3;
uint8_t serial_flag;
//int TCRT;
int main(void)
{	
	//***** 变量 *****//
	u16 adcx;	//ADC电压采集							
	float temp;
	//***** 初始化 *****//
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    //延时函数初始化	  
	TB6612_GPIO_Init();	//电机驱动初始化
	TCRT5000_Init();	//循迹模块初始化
	speed_init();		//对射红外初始化
	Length_Init();		//长度测量初始化
	Adc_Init();		    //ADC初始化
	TIM1_PWM_Init(1999,359);	//电机的PWM 72MHz/2000/360=100Hz  --- 10ms	--- 2k TIM1_PWM_Init(19999,71); //电机的PWM 72MHz/20000/72=50Hz   --- 20ms --- 20k
	TIM_SetCompare1(TIM1,500);	//
	TIM_SetCompare4(TIM1,500);	//
	Tim2_Time_Init();	//定时器2初始化，用于计时1s
	OLED_Init();		//OLED初始化
	OLED_Clear();
	Serial_Init();
	
	while(1)
	{
		/***************
			OLED显示
			电压
			圈数/s
			速度
			长度
		***************/
		adcx=Get_Adc_Average(ADC_Channel_4,10);
		temp=(float)adcx*(3.3/4096);
		sprintf((char *)string,"U:%.2fV",(temp*3));	//分压temp*3
		OLED_ShowString(0,0,string,16);
		sprintf((char *)string,"Speed:%.2fcm/s ",CarSpeed);
		OLED_ShowString(0,2,string,16);
		sprintf((char *)string,"Length:%.2fcm ",t3);
		OLED_ShowString(0,4,string,16);
		sprintf((char *)string,"Num:%d ",Serial_RxPacket[0]);
		OLED_ShowString(0,6,string,16);
		/***************
			黑
			0000直行
			0001右
			0010右
			0011右
			0100左
			0101
			0110直行
			0111右
			1000左
			1001
			1010
			1011右
			1100左
			1101左
			1110左
			1111停止
			
			白
			1111直行
			1110右
			1101右
			1100右
			1011左
			1010
			1001直行
			1000右
			0111左
			0110
			0101
			0100右
			0011左
			0010左
			0001左
			0000停止
		****************/
		if(serial_flag==1)
		{
		if(TCRT_1 == 0 && TCRT_2 == 0 && TCRT_3 == 0 && TCRT_4 == 1)//0001
		{
			stop();
			delay_ms(5);
			Forward();
			delay_ms(10);
			Leftward_Medium();
			delay_ms(140);
			
		}
		else if(TCRT_1 == 1 && TCRT_2 == 0 && TCRT_3 == 0 && TCRT_4 == 0)//1000
		{
			stop();
			delay_ms(5);
			Forward();
			delay_ms(10);
			Rightward_Medium();
			delay_ms(140);
		}
		else if(TCRT_1 == 1 && TCRT_2 == 1 && TCRT_3 == 0 && TCRT_4 == 1)//1101
		{
			stop();
			delay_ms(5);
			Rightward_Medium();
			delay_ms(60);
		}
		else if(TCRT_1 == 1 && TCRT_2 == 1 && TCRT_3 == 1 && TCRT_4 == 0)//1110
		{
			stop();
			delay_ms(5);
			Rightward_Medium();
			delay_ms(100);
		}
		else if(TCRT_1 == 1 && TCRT_2 == 0 && TCRT_3 == 1 && TCRT_4 == 1)//1011
		{
			stop();
			delay_ms(5);
			Leftward_Medium();
			delay_ms(60);
		}
		else if(TCRT_1 == 0 && TCRT_2 == 1 && TCRT_3 == 1 && TCRT_4 == 1)//0111
		{
			stop();
			delay_ms(5);
			Leftward_Medium();
			delay_ms(100);
		}
		else if(TCRT_1 == 1 && TCRT_2 == 1 && TCRT_3 == 0 && TCRT_4 == 0)//1100
		{
			stop();
			delay_ms(5);
			Rightward_Medium();
			delay_ms(60);
		}
		else if(TCRT_1 == 0 && TCRT_2 == 1 && TCRT_3 == 0 && TCRT_4 == 0)//0100
		{
			stop();
			delay_ms(5);
			Rightward_Medium();
			delay_ms(40);
		}
		else if(TCRT_1 == 0 && TCRT_2 == 0 && TCRT_3 == 1 && TCRT_4 == 1)//0011
		{
			stop();
			delay_ms(5);
			Leftward_Medium();
			delay_ms(60);
		}
		else if(TCRT_1 == 0 && TCRT_2 == 0 && TCRT_3 == 1 && TCRT_4 == 0 )//0010
		{
			stop();
			delay_ms(5);
			Leftward_Medium();
			delay_ms(40);
		}
		else if(TCRT_1 == 1 && TCRT_2 == 1 && TCRT_3 == 1 && TCRT_4 == 1)//1111
		{
			stop();
			delay_ms(50);
		}
		else if(TCRT_1 == 0 && TCRT_2 ==0 && TCRT_3 == 0 && TCRT_4 == 0)//0000
		{
			stop();
			delay_ms(50);
			if(Serial_RxPacket[0]==1)
			{
				Leftward_Medium();
				delay_ms(700);
				Forward();
				delay_ms(500);
				while(1){stop();}
			}
			if(Serial_RxPacket[0]==2)
			{
				Rightward_Medium();
				delay_ms(700);
				Forward();
				delay_ms(500);
				while(1){stop();}
			}
		}
		else if(TCRT_1 == 1 && TCRT_2 == 0 && TCRT_3 == 0 && TCRT_4 == 1 )//1001
		{
			Forward();
			delay_ms(50);
		}
	}
	}
}

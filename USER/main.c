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
u8 string[10] = {0};	//OLED�ַ�����ʾ
int t1;
int t2;
float t3;
uint8_t serial_flag;
//int TCRT;
int main(void)
{	
	//***** ���� *****//
	u16 adcx;	//ADC��ѹ�ɼ�							
	float temp;
	//***** ��ʼ�� *****//
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    //��ʱ������ʼ��	  
	TB6612_GPIO_Init();	//���������ʼ��
	TCRT5000_Init();	//ѭ��ģ���ʼ��
	speed_init();		//��������ʼ��
	Length_Init();		//���Ȳ�����ʼ��
	Adc_Init();		    //ADC��ʼ��
	TIM1_PWM_Init(1999,359);	//�����PWM 72MHz/2000/360=100Hz  --- 10ms	--- 2k TIM1_PWM_Init(19999,71); //�����PWM 72MHz/20000/72=50Hz   --- 20ms --- 20k
	TIM_SetCompare1(TIM1,500);	//
	TIM_SetCompare4(TIM1,500);	//
	Tim2_Time_Init();	//��ʱ��2��ʼ�������ڼ�ʱ1s
	OLED_Init();		//OLED��ʼ��
	OLED_Clear();
	Serial_Init();
	
	while(1)
	{
		/***************
			OLED��ʾ
			��ѹ
			Ȧ��/s
			�ٶ�
			����
		***************/
		adcx=Get_Adc_Average(ADC_Channel_4,10);
		temp=(float)adcx*(3.3/4096);
		sprintf((char *)string,"U:%.2fV",(temp*3));	//��ѹtemp*3
		OLED_ShowString(0,0,string,16);
		sprintf((char *)string,"Speed:%.2fcm/s ",CarSpeed);
		OLED_ShowString(0,2,string,16);
		sprintf((char *)string,"Length:%.2fcm ",t3);
		OLED_ShowString(0,4,string,16);
		sprintf((char *)string,"Num:%d ",Serial_RxPacket[0]);
		OLED_ShowString(0,6,string,16);
		/***************
			��
			0000ֱ��
			0001��
			0010��
			0011��
			0100��
			0101
			0110ֱ��
			0111��
			1000��
			1001
			1010
			1011��
			1100��
			1101��
			1110��
			1111ֹͣ
			
			��
			1111ֱ��
			1110��
			1101��
			1100��
			1011��
			1010
			1001ֱ��
			1000��
			0111��
			0110
			0101
			0100��
			0011��
			0010��
			0001��
			0000ֹͣ
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

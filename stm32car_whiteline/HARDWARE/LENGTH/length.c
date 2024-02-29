#include "length.h"
#include "speed.h"
#include "usart.h"
#include "oled.h"
#include "delay.h"
void Length_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//����AFIO��ʱ�ӣ��ⲿ�жϱ��뿪��AFIO��ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//��PA3���ų�ʼ��Ϊ��������
	
	/*AFIOѡ���ж�����*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);//���ⲿ�жϵ�3����ӳ�䵽GPIOA����ѡ��PA3Ϊ�ⲿ�ж�����
	
	/*EXTI��ʼ��*/
	EXTI_InitTypeDef EXTI_InitStructure;						//����ṹ�����
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;					//ѡ�������ⲿ�жϵ�3����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//ָ���ⲿ�ж���ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//ָ���ⲿ�ж���Ϊ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;		//ָ���ⲿ�ж���Ϊ�������½��ش��� 
	EXTI_Init(&EXTI_InitStructure);								//���ṹ���������EXTI_Init������EXTI����
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;						//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;		//ѡ������NVIC��EXTI3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ0
	NVIC_Init(&NVIC_InitStructure);								//���ṹ���������NVIC_Init������NVIC����
	
}
// 1->0->1
extern int t1;
extern int t2;
extern float t3;
u8 string1[10] = {0};	

void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) == SET)		//�ж��Ƿ����ⲿ�ж�3���ߴ������ж�
	{
		delay_ms(30);
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
		{
			t1 = CountSensor1_Get();
		}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 1)
		{
			t2 = CountSensor1_Get();
			t3 = (t2-t1)*1.02;//3.14*6.5/20=1.02
			OLED_Clear();
			sprintf((char *)string1,"Length:%.2fcm ",t3);
			OLED_ShowString(0,4,string1,16);
		}
		
		EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
													//�жϱ�־λ�������
													//�����жϽ��������ϵش�����������������
	}
}




#include "speed.h"
#include "oled.h"
#include "usart.h"

extern int CountSensor_Count;				//ȫ�ֱ��������ڼ���
extern float CarSpeed;
extern int CountSensor_Count1;
int _1s_number = 0;      //��ʱ���жϴ����ļ���ֵ��100�μ�Ϊ1s
float Diameter = 6.5; //С�����ֵ�ֱ��
float pi = 3.14;

void speed_init()
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//����GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//����AFIO��ʱ�ӣ��ⲿ�жϱ��뿪��AFIO��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//��PA5���ų�ʼ��Ϊ��������
	
	/*AFIOѡ���ж�����*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);//���ⲿ�жϵ�5����ӳ�䵽GPIOB����ѡ��PA5Ϊ�ⲿ�ж�����
	
	/*EXTI��ʼ��*/
	EXTI_InitTypeDef EXTI_InitStructure;						//����ṹ�����
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;					//ѡ�������ⲿ�жϵ�14����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//ָ���ⲿ�ж���ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//ָ���ⲿ�ж���Ϊ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//ָ���ⲿ�ж���Ϊ�½��ش��� EXTI_Trigger_Falling
	EXTI_Init(&EXTI_InitStructure);								//���ṹ���������EXTI_Init������EXTI����
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;						//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;		//ѡ������NVIC��EXTI9_5��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//ָ��NVIC��·����ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ0
	NVIC_Init(&NVIC_InitStructure);								//���ṹ���������NVIC_Init������NVIC����
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
	if (EXTI_GetITStatus(EXTI_Line5) == SET)		//�ж��Ƿ����ⲿ�ж�5���ߴ������ж�
	{
		/*��������������������󣬿��ٴ��ж����ŵ�ƽ���Ա��ⶶ��*/
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)
		{
			CountSensor_Count ++;					//����ֵ����һ��
			CountSensor_Count1 ++;
		}
		EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE5�ϵ��жϱ�־λ  
													//�жϱ�־λ�������
													//�����жϽ��������ϵش�����������������
	}
}
// ��ʱ���жϺ��������Ը��Ƶ�ʹ�����ĵط� 1s
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



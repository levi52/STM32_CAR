#ifndef __TCRT_H
#define __TCRT_H
#include "sys.h"
//��----->��
//TCRT_1 TCRT_2 TCRT_3 TCRT_4
#define TCRT_1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//��ȡ����Թ�
#define TCRT_2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)//��ȡ����Թ�
#define TCRT_3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//��ȡ����Թ� 
#define TCRT_4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//��ȡ����Թ�
#define TCRT_5  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)//��ȡ����Թ�

void TCRT5000_Init(void);
#endif


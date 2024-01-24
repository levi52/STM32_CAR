#ifndef __TCRT_H
#define __TCRT_H
#include "sys.h"
//左----->右
//TCRT_1 TCRT_2 TCRT_3 TCRT_4
#define TCRT_1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取红外对管
#define TCRT_2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)//读取红外对管
#define TCRT_3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//读取红外对管 
#define TCRT_4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//读取红外对管
#define TCRT_5  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)//读取红外对管

void TCRT5000_Init(void);
#endif


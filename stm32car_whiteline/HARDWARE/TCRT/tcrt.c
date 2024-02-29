#include "tcrt.h"

/**
* @brief 红外循迹四个GPIO的初始化
* @param 
* @return 
*/
void TCRT5000_Init(void) //IO初始化
{ 
 	
    /*开启时钟*/
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTB时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //默认下拉	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}


#include "tcrt.h"

/**
* @brief ����ѭ���ĸ�GPIO�ĳ�ʼ��
* @param 
* @return 
*/
void TCRT5000_Init(void) //IO��ʼ��
{ 
 	
    /*����ʱ��*/
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTBʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //Ĭ������	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}


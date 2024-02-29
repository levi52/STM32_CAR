#include "motor.h"

/**
* @brief ��ʼTB6612�� AIN1 AIN2 BIN1 BIN2
* @brief ����AIN1--PB13 AIN2-PB12 BIN1--PB1  BIN2--PB0
* @param 
* @return 
*/
void TB6612_GPIO_Init(void)
{ 
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //����GPIOBʱ��
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_0 | GPIO_Pin_1; //PB0 PB1 PB12 PB13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO 50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_0 | GPIO_Pin_1);
}
//ֱ��
void Forward(void)
{
	AIN1 = 1;
	AIN2 = 0;
	BIN1 = 1;
	BIN2 = 0;
	TIM_SetCompare4(TIM1,980); //A
	TIM_SetCompare1(TIM1,980); //B
}
///**
//* @brief 
//* @param 
//* @return 
//*/
void Backward(void)
{
	AIN1 = 0;
	AIN2 = 1;
	BIN1 = 0;
	BIN2 = 1;
	TIM_SetCompare4(TIM1,700); //A
	TIM_SetCompare1(TIM1,700); //B
}
/**
* @brief 
* @param 
* @return 
*/
//������ת
void Rightward_Super(void)
{
	AIN1 = 0;
	AIN2 = 1;
	BIN1 = 1;
	BIN2 = 0;
	TIM_SetCompare4(TIM1,100); //A
	TIM_SetCompare1(TIM1,100); //B
}
//������ת
void Rightward_Fast(void)
{
	AIN1 = 0;
	AIN2 = 1;
	BIN1 = 1;
	BIN2 = 0;
	TIM_SetCompare4(TIM1,700); //A
	TIM_SetCompare1(TIM1,700); //B
}
//������ת
void Rightward_Medium(void)
{
	AIN1 = 0;
	AIN2 = 1;
	BIN1 = 1;
	BIN2 = 0;
	TIM_SetCompare4(TIM1,980); //A
	TIM_SetCompare1(TIM1,980); //B
}
//������ת
void Rightward_Slow(void)
{
	AIN1 = 0;
	AIN2 = 1;
	BIN1 = 1;
	BIN2 = 0;
	TIM_SetCompare4(TIM1,1000); //A
	TIM_SetCompare1(TIM1,1000); //B
}
/**
* @brief  
* @param 
* @return 
*/
//������ת
void Leftward_Super(void)
{
	AIN1 = 1;
	AIN2 = 0;
	BIN1 = 0;
	BIN2 = 1;
	TIM_SetCompare4(TIM1,100); //A
	TIM_SetCompare1(TIM1,100); //B
}
//������ת
void Leftward_Fast(void)
{
	AIN1 = 1;
	AIN2 = 0;
	BIN1 = 0;
	BIN2 = 1;
	TIM_SetCompare4(TIM1,700); //A
	TIM_SetCompare1(TIM1,700); //B
}
void Leftward_Medium(void)
{
	AIN1 = 1;
	AIN2 = 0;
	BIN1 = 0;
	BIN2 = 1;
	TIM_SetCompare4(TIM1,980); //A
	TIM_SetCompare1(TIM1,980); //B
}
void Leftward_Slow(void)
{
	AIN1 = 1;
	AIN2 = 0;
	BIN1 = 0;
	BIN2 = 1;
	TIM_SetCompare4(TIM1,1000); //A
	TIM_SetCompare1(TIM1,1000); //B
}

void stop(void)
{
	AIN1 = 0;
	AIN2 = 0;
	BIN1 = 0;
	BIN2 = 0;	
}


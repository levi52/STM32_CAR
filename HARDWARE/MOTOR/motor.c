#include "motor.h"

/**
* @brief 初始TB6612的 AIN1 AIN2 BIN1 BIN2
* @brief 连接AIN1--PB13 AIN2-PB12 BIN1--PB1  BIN2--PB0
* @param 
* @return 
*/
void TB6612_GPIO_Init(void)
{ 
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //开启GPIOB时钟
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_0 | GPIO_Pin_1; //PB0 PB1 PB12 PB13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO 50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_0 | GPIO_Pin_1);
}
//直走
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
//快速右转
void Rightward_Super(void)
{
	AIN1 = 0;
	AIN2 = 1;
	BIN1 = 1;
	BIN2 = 0;
	TIM_SetCompare4(TIM1,100); //A
	TIM_SetCompare1(TIM1,100); //B
}
//快速右转
void Rightward_Fast(void)
{
	AIN1 = 0;
	AIN2 = 1;
	BIN1 = 1;
	BIN2 = 0;
	TIM_SetCompare4(TIM1,700); //A
	TIM_SetCompare1(TIM1,700); //B
}
//中速右转
void Rightward_Medium(void)
{
	AIN1 = 0;
	AIN2 = 1;
	BIN1 = 1;
	BIN2 = 0;
	TIM_SetCompare4(TIM1,980); //A
	TIM_SetCompare1(TIM1,980); //B
}
//慢速右转
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
//快速左转
void Leftward_Super(void)
{
	AIN1 = 1;
	AIN2 = 0;
	BIN1 = 0;
	BIN2 = 1;
	TIM_SetCompare4(TIM1,100); //A
	TIM_SetCompare1(TIM1,100); //B
}
//快速左转
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


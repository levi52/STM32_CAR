#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define AIN1 PBout(13)// PB13
#define AIN2 PBout(12)// PB12
#define BIN1 PBout(1)// PB1
#define BIN2 PBout(0)// PB0

void TB6612_GPIO_Init(void);
void Forward(void);
void Backward(void);
void Rightward_Super(void);
void Rightward_Fast(void);
void Rightward_Medium(void);
void Rightward_Slow(void);
void Leftward_Super(void);
void Leftward_Fast(void);
void Leftward_Medium(void);
void Leftward_Slow(void);
void stop(void);

#endif

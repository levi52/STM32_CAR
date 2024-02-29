#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr,u16 pac);
void Tim2_Time_Init(void);
void Time4_Init(void);
#endif

#ifndef __KEY
#define __KEY

#include "gd32f4xx.h"
#include "systick.h"
#include "usart.h"
void Key_Config(void);
uint8_t Key_scan(void);
void EXTI_Config(void);


#endif 

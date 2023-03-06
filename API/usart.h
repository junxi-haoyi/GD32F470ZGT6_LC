#ifndef __USART
#define __USART

#include "gd32f4xx.h"
#include "main.h"

extern uint8_t receive_char[4096];
extern uint16_t char_flag;
extern uint8_t char_num;
extern uint8_t led_flag;

void USART_Config(void);

#endif



#ifndef __LED
#define __LED

#include "gd32f4xx.h"

#define BIT_ADDR(byte_offset,bitnum) (volatile unsigned long*)(0x42000000+(byte_offset*32)+(bitnum*4))


void LED_GPIOConfig(void);
void GPIO_register(void);


#endif // !__LED

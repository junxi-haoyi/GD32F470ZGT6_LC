/*!
    \file    main.c
    \brief   led spark with systick

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
    \version 2022-03-09, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


#include "main.h"


int main(void)
{ 
   SystemInit();
   EXTI_Config();
   systick_config();
   USART_Config();
   printf("karly:5401153A\n\r");
   GPIO_register();
   Key_Config();
   TIM0_Config();
   TIM1_PWM_Config();
   DMA_Config();
   
    while(1) 
    {

      if(char_flag)
      {
         printf("1");
      }





      // if(Key_scan())
      //   {

      //       printf("1");
      //       j=!j;
      //   }

      // if(1&j)
      //   {
      //       *(BIT_ADDR(0x21014,3))|=1;
      //   }
      //   else *(BIT_ADDR(0x21014,3))&=0;




      //   if(Key_scan())
      //   {

      //       printf("1");
      //       if(j>=0x10)
      //       {
      //          j=1;
      //          *(BIT_ADDR(0x21014,3))&=0;
      //          *(BIT_ADDR(0x20c14,7))&=0;
      //          *(BIT_ADDR(0x21814,3))&=0;
      //          *(BIT_ADDR(0x20014,5))&=0;
      //       }
      //       else 
      //       {
      //          j<<=1;
      //       }
      //   }

      //    if(2&j)
      //   {
      //       *(BIT_ADDR(0x21014,3))|=1;
      //   }
      //   //else *(BIT_ADDR(0x21014,3))&=0;

      //   if(4&j)
      //   {
      //       *(BIT_ADDR(0x20c14,7))|=1;
      //   }
      //   //else *(BIT_ADDR(0x20c14,7))&=0;

      //   if(8&j)
      //   {
      //       *(BIT_ADDR(0x21814,3))|=1;
      //   }
      //   //else *(BIT_ADDR(0x21814,3))&=0;

      //   if(16&j)
      //   {
      //       *(BIT_ADDR(0x20014,5))|=1;
      //   }
      //   //else *(BIT_ADDR(0x20014,5))&=0;




      //   if(LED_Time<500)
      //   {
      //       *(BIT_ADDR(0x21014,3))|=1;
      //   }
      //   else *(BIT_ADDR(0x21014,3))&=0;

      //   if(LED_Time>=500&&LED_Time<1000)
      //   {
      //       *(BIT_ADDR(0x20c14,7))|=1;
      //   }
      //   else *(BIT_ADDR(0x20c14,7))&=0;

      //   if(LED_Time>=1000&&LED_Time<1500)
      //   {
      //       *(BIT_ADDR(0x21814,3))|=1;
      //   }
      //   else *(BIT_ADDR(0x21814,3))&=0;

      //   if(LED_Time>=1500&&LED_Time<2000)
      //   {
      //       *(BIT_ADDR(0x20014,5))|=1;
      //   }
      //   else *(BIT_ADDR(0x20014,5))&=0;



      //   if(LED_Time<500)
      //   {
      //       gpio_bit_set(GPIOE,GPIO_PIN_3);
      //   }
      //   else gpio_bit_reset(GPIOE,GPIO_PIN_3);

      //   if(LED_Time>=500&&LED_Time<1000)
      //   {
      //       gpio_bit_set(GPIOD,GPIO_PIN_7);
      //   }
      //   else gpio_bit_reset(GPIOD,GPIO_PIN_7);

      //   if(LED_Time>=1000&&LED_Time<1500)
      //   {
      //       gpio_bit_set(GPIOG,GPIO_PIN_3);
      //   }
      //   else gpio_bit_reset(GPIOG,GPIO_PIN_3);

      //   if(LED_Time>=1500&&LED_Time<2000)
      //   {
      //       gpio_bit_set(GPIOA,GPIO_PIN_5);
      //   }
      //   else 
      //   {
      //       gpio_bit_reset(GPIOA,GPIO_PIN_5);

      //   }
        //task 1 !!!

        //if(LED_Time>=2000)LED_Time=0;   

     }
}

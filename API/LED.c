#include "LED.h"

void LED_GPIOConfig(void)
{
    rcu_periph_clock_enable(RCU_GPIOE);
    //gpio_deinit(GPIOE);
    gpio_mode_set(GPIOE,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLDOWN,GPIO_PIN_3);
    gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_25MHZ,GPIO_PIN_3);

    rcu_periph_clock_enable(RCU_GPIOD);
    //gpio_deinit(GPIOD);
    gpio_mode_set(GPIOD,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_PIN_7);
    gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_25MHZ,GPIO_PIN_7);

    rcu_periph_clock_enable(RCU_GPIOG);
    //gpio_deinit(GPIOG);
    gpio_mode_set(GPIOG,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_PIN_3);
    gpio_output_options_set(GPIOG,GPIO_OTYPE_PP,GPIO_OSPEED_25MHZ,GPIO_PIN_3);

    rcu_periph_clock_enable(RCU_GPIOA);
    //gpio_deinit(GPIOA);
    gpio_mode_set(GPIOA,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_PIN_5);
    gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_25MHZ,GPIO_PIN_5);

    

}

void GPIO_register(void)
{
    RCU_AHB1EN&=0;
    
    RCU_AHB1EN|=((1<<0)|(1<<3)|(1<<4)|(1<<6));
    //RCU_AHB1EN|=(0x01011001);

    //0x01011001

    //GPIOA
    *(unsigned int*)(GPIOA+0x00)&=~GPIO_CTL_CTL5;
    *(unsigned int*)(GPIOA+0x00)|=(GPIO_CTL_CTL5&(0x01<<2*5));

    *(unsigned int*)(GPIOA+0x04)|=(GPIO_OMODE_OM5&(0x0<<5));

    *(unsigned int*)(GPIOA+0x08)&=~GPIO_OSPD_OSPD5;
    *(unsigned int*)(GPIOA+0x08)|=(GPIO_OSPD_OSPD5&(0x01<<2*5));
    //

    //GPIOG
    *(unsigned int*)(GPIOG+0x00)&=~GPIO_CTL_CTL3;
    *(unsigned int*)(GPIOG+0x00)|=(GPIO_CTL_CTL3&(0x01<<2*3));

    *(unsigned int*)(GPIOG+0x04)|=(GPIO_OMODE_OM3&(0x0<<3));

    *(unsigned int*)(GPIOG+0x08)&=GPIO_OSPD_OSPD3;
    *(unsigned int*)(GPIOG+0x08)|=(GPIO_OSPD_OSPD3&(0x01<<2*3));
    //

    //GPIOD
    *(unsigned int*)(GPIOD+0x00)&=~GPIO_CTL_CTL7;
    *(unsigned int*)(GPIOD+0x00)|=(GPIO_CTL_CTL7&(0x01<<2*7));

    *(unsigned int*)(GPIOD+0x04)|=(GPIO_OMODE_OM7&(0x0<<7));

    *(unsigned int*)(GPIOD+0x08)&=~GPIO_OSPD_OSPD7;
    *(unsigned int*)(GPIOD+0x08)|=(GPIO_OSPD_OSPD7&(0x01<<2*7));
    //

    //GPIOE
    *(unsigned int*)(GPIOE+0x00)|=~GPIO_CTL_CTL3;
    *(unsigned int*)(GPIOE+0x00)|=(GPIO_CTL_CTL3&(0x01<<2*3));

    *(unsigned int*)(GPIOE+0x04)|=(GPIO_OMODE_OM3&(0x0<<3));

    *(unsigned int*)(GPIOE+0x08)|=~GPIO_OSPD_OSPD3;
    *(unsigned int*)(GPIOE+0x08)|=(GPIO_OSPD_OSPD3&(0x01<<2*3));
    //
}







#include "key.h"

static uint8_t flag=0;
static void NVIC_Config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(EXTI0_IRQn,2,2);
}



void EXTI_Config(void)
{
    RCU_APB2EN|=RCU_APB2EN_SYSCFGEN;
    RCU_AHB1EN|=RCU_AHB1EN_PAEN;

    NVIC_Config();

    SYSCFG_EXTISS0&=~SYSCFG_EXTISS0_EXTI0_SS;//EXTI0 source selection

    EXTI_INTEN|=EXTI_INTEN_INTEN0;
    EXTI_RTEN|=EXTI_RTEN_RTEN0;//rising trigger enable
    EXTI_FTEN&=~EXTI_FTEN_FTEN0;//falling trigger enable

}

void EXTI0_IRQHandler(void)
{
    
    if(EXTI_PD&EXTI_PD_PD0)
    {
        if(Key_scan())
        {
            flag=!flag;
        }

        if(flag)
        {
            GPIO_OCTL(GPIOE)|=GPIO_OCTL_OCTL3;
        }
        else{
            GPIO_OCTL(GPIOE)&=~GPIO_OCTL_OCTL3;    
        }
    }

    EXTI_PD=EXTI_PD_PD0;
    //exti_interrupt_flag_clear(EXTI_0);
    printf("key=%d",flag);

}






void Key_Config(void)
{
    RCU_AHB1EN|=0x1;

    GPIO_CTL(GPIOA)&=~GPIO_CTL_CTL0;

    GPIO_PUD(GPIOA)&=~GPIO_PUD_PUD0;
    GPIO_PUD(GPIOA)|=(GPIO_PUD_PUD0&0xe);
    
}



uint8_t Key_scan(void)
{
    
    if((GPIO_ISTAT(GPIOA)&0x1)==1)
    {
        delay_1ms(50);
        if((GPIO_ISTAT(GPIOA)&0x1)==1)
        {
            while((GPIO_ISTAT(GPIOA)&0x1)==1);
            return 1;
        }
    }
    return 0;
}





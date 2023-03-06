#include "TIM.h"

static uint8_t flag=1;
static uint8_t flag_PWM;
static void NVICTIM_Config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(TIMER0_UP_TIMER9_IRQn,1,1);
}

static void NVIC_Config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(TIMER1_IRQn,0,1);
}

void TIM0_Config(void)
{
    RCU_APB2EN|=0x1;//enable APB2

    RCU_CFG1|=RCU_CFG1_TIMERSEL;//CK_TIMER0 200MHz
    NVICTIM_Config();
    //TIMER_CTL0(TIMER0)&=~TIMER_CTL0_ARSE;//enable shadow register
    TIMER_CTL0(TIMER0)&=~TIMER_CTL0_DIR;//up count
    TIMER_CTL0(TIMER0)&=~TIMER_CTL0_SPM;//disable once tpdate
    TIMER_CTL0(TIMER0)&=~TIMER_CTL0_UPDIS;//enable update event
    TIMER_SMCFG(TIMER0)&=~TIMER_SMCFG_SMC;//internal clk selection
    TIMER_DMAINTEN(TIMER0)|=TIMER_DMAINTEN_UPIE;//enable update interrupt
    TIMER_CTL0(TIMER0)|=TIMER_CTL0_CEN;//enable TIM0
    TIMER_INTF(TIMER0)&=~TIMER_INTF_UPIF;

    TIMER_PSC(TIMER0)=(uint32_t)200-1;
    TIMER_CAR(TIMER0)=(uint32_t)10000-1;
    TIMER_CREP(TIMER0)=(uint32_t)100-1;


}

void TIM1_PWM_Config(void)
{
    RCU_APB1EN|=RCU_APB1EN_TIMER1EN;

    RCU_AHB1EN|=RCU_AHB1EN_PAEN;
    GPIO_CTL(GPIOA)&=~GPIO_CTL_CTL5;
    GPIO_CTL(GPIOA)|=(GPIO_CTL_CTL5&(2<<5*2));
    GPIO_OMODE(GPIOA)&=~GPIO_OMODE_OM5;
    GPIO_OSPD(GPIOA)|=GPIO_OSPD_OSPD5;
    GPIO_AFSEL0(GPIOA)&=~GPIO_AFSEL0_SEL5;
    GPIO_AFSEL0(GPIOA)|=(GPIO_AFSEL0_SEL5&(0x1<<5*4));

    NVIC_Config();
    
    TIMER_SMCFG(TIMER1)&=~TIMER_SMCFG_SMC;//internal clk selection


    RCU_CFG1|=RCU_CFG1_TIMERSEL;//CK_TIMER0 200MHz
    TIMER_DMAINTEN(TIMER1)|=TIMER_DMAINTEN_CH0IE;//enable com interrupt
    TIMER_CTL0(TIMER1)&=~TIMER_CTL0_SPM;//disable SPM
    TIMER_DMAINTEN(TIMER1)&=~TIMER_DMAINTEN_UPIE;//disable updata interrupt
    TIMER_CTL0(TIMER1)&=~TIMER_CTL0_DIR;//up count

    TIMER_CHCTL0(TIMER1)&=~TIMER_CHCTL0_CH0MS;//output compare mode
    TIMER_CHCTL0(TIMER1)|=TIMER_CHCTL0_CH0COMSEN;//enable compare shadow register
    TIMER_CHCTL0(TIMER1)&=~TIMER_CHCTL0_CH0COMCTL;
    TIMER_CHCTL0(TIMER1)|=(TIMER_CHCTL0_CH0COMCTL&(0x6<<4*1));
    TIMER_CH0CV(TIMER1)=50;
    TIMER_CHCTL2(TIMER1)|=TIMER_CHCTL2_CH0EN;


    TIMER_PSC(TIMER1)=(uint32_t)1000-1;//TIMER1 0.1MHz
    TIMER_CAR(TIMER1)=(uint32_t)1000-1;

    //timer_channel_output_config()

    TIMER_CTL0(TIMER1)|=TIMER_CTL0_CEN;//enable TIMER

}

void TIMER1_IRQHandler(void)
{
    //printf("1");
    if(TIMER_CH0CV(TIMER1)>=1000-1)
    {
        flag_PWM=1;
    }
    else if(TIMER_CH0CV(TIMER1)<=0){
        flag_PWM=0;
    }

    if(flag_PWM)
    {
        TIMER_CH0CV(TIMER1)-=2;
    }
    else
    {
        TIMER_CH0CV(TIMER1)+=2;
    }
    
    TIMER_INTF(TIMER1)&=~TIMER_INTF_CH0IF;//clear interrupt flag
}

void TIMER0_UP_TIMER9_IRQHandler(void)
{
    if(TIMER_INTF(TIMER0)&TIMER_INTF_UPIF)
    {
        if(flag)
        {
        GPIO_OCTL(GPIOD)|=GPIO_OCTL_OCTL7;
        }
        else
        {
        GPIO_OCTL(GPIOD)&=~GPIO_OCTL_OCTL7;
        }
    }
    flag=!flag;
    TIMER_INTF(TIMER0) &= (~(uint32_t)TIMER_INT_FLAG_UP);
    //timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_UP);
    //TIMER_INTF(TIMER0)&=~TIMER_INTF_UPIF;
    //delay_us(1);
}







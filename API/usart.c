#include "usart.h"


uint8_t receive_char[4096];
uint16_t char_flag=0;
uint8_t char_num=0;
static uint8_t command1[]={"led_on"};
static uint8_t command2[]={"led_off"};
uint8_t led_flag=0;

// static void NVIC_Config(void)
// {
//     nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
//     nvic_irq_enable(USART0_IRQn,3,0);
// }



void USART_Config(void)
{
    RCU_AHB1EN|=RCU_AHB1EN_PAEN;
    RCU_APB2EN|=RCU_APB2EN_USART0EN;

    GPIO_CTL(GPIOA)&=~GPIO_CTL_CTL10;
    GPIO_CTL(GPIOA)|=(GPIO_CTL_CTL10&(0x2U<<(10U*2)));
    GPIO_CTL(GPIOA)&=~GPIO_CTL_CTL9;
    GPIO_CTL(GPIOA)|=(GPIO_CTL_CTL9&(0x2U<<(9U*2)));

    GPIO_OMODE(GPIOA)&=~GPIO_OMODE_OM9;
    GPIO_OMODE(GPIOA)&=~GPIO_OMODE_OM10;

    //GPIO_OSPD(GPIOA)|=GPIO_OSPD_OSPD10;
    GPIO_OSPD(GPIOA)|=GPIO_OSPD_OSPD9;

    GPIO_AFSEL1(GPIOA)&=~GPIO_AFSEL1_SEL9;
    GPIO_AFSEL1(GPIOA)|=(GPIO_AFSEL1_SEL9&(GPIO_AF_7<<1*4));
    GPIO_AFSEL1(GPIOA)&=~GPIO_AFSEL1_SEL10;
    GPIO_AFSEL1(GPIOA)|=(GPIO_AFSEL1_SEL10&(GPIO_AF_7<<2*4));

    //NVIC_Config();

    usart_baudrate_set(USART0,115200);

    USART_CTL0(USART0)&=~USART_CTL0_WL;

    USART_CTL0(USART0)&=~USART_CTL0_PCEN;

    USART_CTL1(USART0)&=~USART_CTL1_STB;
 
    USART_CTL0(USART0)|=USART_CTL0_UEN;

    USART_CTL0(USART0)|=USART_CTL0_TEN;
    USART_CTL0(USART0)|=USART_CTL0_REN;

    //USART_CTL0(USART0)|=USART_CTL0_RBNEIE;
    USART_CTL0(USART0)|=USART_CTL0_IDLEIE; 
    //USART_CTL2(USART0)|=USART_CTL2_DENT;//enable DMA tansmit
    USART_CTL2(USART0)|=USART_CTL2_DENR;//enable DMA recieve
    //usart_dma_receive_config(USART0,USART_DENR_ENABLE);
}



int fputc(int ch,FILE *f)
{
    USART_DATA(USART0)=(USART_DATA_DATA&ch);
    while((USART_STAT0(USART0)&USART_STAT0_TBE)==0);
    return ch;
}




// void USART0_IRQHandler(void)
// {
//     // if((USART_STAT0(USART0)&USART_STAT0_RBNE)==USART_STAT0_RBNE)
//     // {
//     //     printf("2");
//     //     receive_char[char_num++]=USART_DATA(USART0);
//     //     USART_STAT0(USART0)&=~USART_STAT0_RBNE;
//     // }
    
//     if((USART_STAT0(USART0)&USART_STAT0_IDLEF)==USART_STAT0_IDLEF)
//     {
//         printf("1\n\r");
//         receive_char[char_num]='\0';
//         USART_DATA(USART0);//clear update flag only after read USART data attention!!!! 
//         USART_STAT0(USART0)&=~(uint32_t)USART_STAT0_IDLEF;
//         //usart_data_receive(USART0);
//         char_flag=1;
//     }

//     if(char_flag)
//     {
//         uint8_t i; 
//         char_flag=0; 
//         for(i=0;i<char_num+1;i++)
//         {
//             if(receive_char[i]==command1[i])
//             {
//                 printf("3");
//                 if(i>=char_num-3)
//                 {
//                     led_flag=1;
//                     printf("6");
//                 }
//                 continue;
//             }
//             else if(receive_char[i]==command2[i])
//             {
//                 if(i>=char_num-3)led_flag=0;
//                 continue;
//             }
//             else
//             {
//                 memset(receive_char,0,char_num);
//                 char_num=0;
//                 break;
//             }
//         }
//         if(led_flag)
//         {
//             printf("4");
//             gpio_bit_set(GPIOG,GPIO_PIN_3);
//         }
//         else
//         {
//             gpio_bit_reset(GPIOG,GPIO_PIN_3);
//         }
//     }

// }



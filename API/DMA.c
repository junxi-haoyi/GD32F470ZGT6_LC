#include "DMA.h"

static uint8_t command1[]={"led_on"};
static uint8_t command2[]={"led_off"};

static void NVIC_Config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE3_SUB1);
    nvic_irq_enable(DMA1_Channel2_IRQn,0,1);
}



void DMA_Config(void)
{
    RCU_AHB1EN|=RCU_AHB1EN_DMA1EN;//enable DMA1

    DMA_CH2CTL(DMA1)&=~DMA_CHXCTL_TM;//set periph to memorizer

    DMA_CH2CTL(DMA1)&=~DMA_CHXCTL_PWIDTH;//periph data size with 1 byte

    DMA_CH2CTL(DMA1)&=~DMA_CHXCTL_CMEN;//disable circuclation mode
    DMA_CH2CTL(DMA1)&=~DMA_CHXCTL_TFCS;//transmit controler selection with DMA
    DMA_CH2CTL(DMA1)|=DMA_CHXCTL_FTFIE;//enable transmition complete interrupt
    DMA_CH2CNT(DMA1)=10;//DMA1 CNT
    DMA_CH2PADDR(DMA1)=(uint32_t)&USART_DATA(USART0);//periph address
    DMA_CH2CTL(DMA1)&=~DMA_CHXCTL_MBS;//select memory 0 as memorizer
    DMA_CH2M0ADDR(DMA1)=(uint32_t)receive_char;
    DMA_CH2CTL(DMA1)&=~DMA_CHXCTL_PNAGA;//periph address fixed
    DMA_CH2CTL(DMA1)|=DMA_CHXCTL_MNAGA;//increasing memorizer adress
    DMA_CH2CTL(DMA1)|=DMA_CHXCTL_PRIO;

    DMA_CH2CTL(DMA1)&=~DMA_CHXCTL_PERIEN;
    DMA_CH2CTL(DMA1)|=(DMA_CHXCTL_PERIEN&((uint32_t)0x4<<25));//enable channel 100

    DMA_CH2CTL(DMA1)|=DMA_CHXCTL_CHEN;//enable channel 2
    NVIC_Config();




    //dma_single_data_parameter_struct dma_init_struct;                // DMA单数据结构体
    // rcu_periph_clock_enable(RCU_DMA1);														// 开启DMA时钟
  	
	 //dma_deinit(DMA1,DMA_CH2);																	// 初始化DMA通道
	
	// // /* 配置DMA初始化参数 */
	//  dma_init_struct.periph_addr 				 = (uint32_t)&USART_DATA(USART0);   // 外设地址                       
    //  dma_init_struct.periph_inc					 = DMA_PERIPH_INCREASE_DISABLE;        // 不使用增量模式，为固定模式  
    //  dma_init_struct.memory0_addr 			 = (uint32_t)receive_char;              // 内存地址          
    //  dma_init_struct.memory_inc 				 = DMA_MEMORY_INCREASE_ENABLE;      	 // 增量模式                    
    //  dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;         		 // 一次传输长度8bit         
    //  dma_init_struct.circular_mode 			 = DMA_CIRCULAR_MODE_DISABLE;      		 // 关闭循环模式               
    //  dma_init_struct.direction 					 = DMA_PERIPH_TO_MEMORY;          		 // 外设到内存                  
    //  dma_init_struct.number 						 = 10;         			 // 要传输的数据量                     
    //  dma_init_struct.priority						 = DMA_PRIORITY_ULTRA_HIGH;  					 // 超高优先级
	// //  /* 初始化DMA结构体 */
	// dma_single_data_mode_init(DMA1,DMA_CH2,&dma_init_struct);
	
	// //  /* 使能通道外设 */
	//  dma_channel_subperipheral_select(DMA1,DMA_CH2,DMA_SUBPERI4);
	// //  /* 使能DMA通道 */
	//  dma_channel_enable(DMA1,DMA_CH2);
	
	//  /* 使能DMA通道中断 */
	// dma_interrupt_enable(DMA1,DMA_CH2,DMA_CHXCTL_FTFIE);
	//  /* 配置中断优先级 */
	// NVIC_Config(); 
	//  /* 使能串口DMA接收 */
	// usart_dma_receive_config(USART0,USART_DENR_ENABLE);
}


void DMA1_Channel2_IRQHandler(void)
{
    printf("DMA interrupt");
    printf("%s",receive_char);
    if(dma_interrupt_flag_get(DMA1,DMA_CH2,DMA_INT_FLAG_FTF)==SET)
    {
        DMA_INTC0(DMA1)|=(DMA_INTC_FTFIFC<<16);
        char_flag=1;
    }

    if(char_flag)
    {
        uint8_t i; 
        char_flag=0; 
        for(i=0;i<char_num+1;i++)
        {
            if(receive_char[i]==command1[i])
            {
                printf("%d",i);
                if(receive_char[i]=='n')
                {
                    led_flag=1;
                    printf("6");
                }
                continue;
            }
            else if(receive_char[i]==command2[i])
            {
                if(receive_char[i]=='f')led_flag=0;
                continue;
            }
            else
            {
                memset(receive_char,0,char_num);
                char_num=0;
                break;
            }
        }
        if(led_flag)
        {
            printf("4");
            gpio_bit_set(GPIOG,GPIO_PIN_3);
        }
        else
        {
            gpio_bit_reset(GPIOG,GPIO_PIN_3);
        }
    }
}
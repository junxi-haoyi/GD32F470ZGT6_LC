/*!
    \file  gd32f403_dma.c
    \brief DMA driver
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-02-10, V1.0.0, firmware for GD32F403
*/

#include "gd32f403_dma.h"

#define DMA_WRONG_HANDLE        while(1){}

static ErrStatus dma_periph_and_channel_check(uint32_t dma_periph, dma_channel_enum channelx);

/*!
    \brief      deinitialize DMA a channel registers 
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel is deinitialized
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     none
*/
void dma_deinit(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    /* disable DMA a channel */
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CHEN;
    /* reset DMA channel registers */
    DMA_CHCTL(dma_periph, channelx) = DMA_CHCTL_RESET_VALUE;
    DMA_CHCNT(dma_periph, channelx) = DMA_CHCNT_RESET_VALUE;
    DMA_CHPADDR(dma_periph, channelx) = DMA_CHPADDR_RESET_VALUE;
    DMA_CHMADDR(dma_periph, channelx) = DMA_CHMADDR_RESET_VALUE;
    DMA_INTC(dma_periph) |= DMA_FLAG_ADD(DMA_CHINTF_RESET_VALUE, channelx);
}

/*!
    \brief      initialize DMA channel
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel is initialized
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  init_struct: the data needed to initialize DMA channel
                  periph_addr: peripheral base address
                  periph_width: DMA_PERIPHERAL_WIDTH_8BIT, DMA_PERIPHERAL_WIDTH_16BIT, DMA_PERIPHERAL_WIDTH_32BIT
                  periph_inc: DMA_PERIPH_INCREASE_ENABLE, DMA_PERIPH_INCREASE_DISABLE 
                  memory_addr: memory base address
                  memory_width: DMA_MEMORY_WIDTH_8BIT, DMA_MEMORY_WIDTH_16BIT, DMA_MEMORY_WIDTH_32BIT
                  memory_inc: DMA_MEMORY_INCREASE_ENABLE, DMA_MEMORY_INCREASE_DISABLE
                  direction: DMA_PERIPHERAL_TO_MEMORY, DMA_MEMORY_TO_PERIPHERAL
                  number: the number of remaining data to be transferred by the DMA
                  priority: DMA_PRIORITY_LOW, DMA_PRIORITY_MEDIUM, DMA_PRIORITY_HIGH, DMA_PRIORITY_ULTRA_HIGH
    \param[out] none
    \retval     none
*/
void dma_init(uint32_t dma_periph, dma_channel_enum channelx, dma_parameter_struct init_struct)
{
    uint32_t ctl;
    
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    /* configure peripheral base address */
    DMA_CHPADDR(dma_periph, channelx) = init_struct.periph_addr;
    
    /* configure memory base address */
    DMA_CHMADDR(dma_periph, channelx) = init_struct.memory_addr;
    
    /* configure the number of remaining data to be transferred */
    DMA_CHCNT(dma_periph, channelx) = (init_struct.number & DMA_CHANNEL_CNT_MASK);
    
    /* configure peripheral transfer width,memory transfer width, */
    ctl = DMA_CHCTL(dma_periph, channelx);
    ctl &= ~(DMA_CHXCTL_PWIDTH | DMA_CHXCTL_MWIDTH | DMA_CHXCTL_PRIO);
    ctl |= (init_struct.periph_width | init_struct.memory_width | init_struct.priority);
    DMA_CHCTL(dma_periph, channelx) = ctl;

    /* configure peripheral increasing mode */
    if(DMA_PERIPH_INCREASE_ENABLE == init_struct.periph_inc){
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
    }else{
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_PNAGA;
    }

    /* configure memory increasing mode */
    if(DMA_MEMORY_INCREASE_ENABLE == init_struct.memory_inc){
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MNAGA;
    }else{
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MNAGA;
    }
    
    /* configure the direction of  data transfer */
    if(DMA_PERIPHERAL_TO_MEMORY == init_struct.direction){
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_DIR;
    }else{
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_DIR;
    } 
}

/*!
    \brief      enable DMA circulation mode  
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     none 
*/
void dma_circulation_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CMEN;
}

/*!
    \brief      disable DMA circulation mode  
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     none 
*/
void dma_circulation_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CMEN;
}

/*!
    \brief      enable memory to memory mode
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     none
*/
void dma_memory_to_memory_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_M2M;
}

/*!
    \brief      disable memory to memory mode
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     none
*/
void dma_memory_to_memory_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_M2M;
}

/*!
    \brief      enable DMA channel 
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     none 
*/
void dma_channel_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CHEN;
}

/*!
    \brief      disable DMA channel 
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     none 
*/
void dma_channel_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CHEN;
}

/*!
    \brief      set DMA peripheral base address  
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set peripheral base address 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  address: peripheral base address
    \param[out] none
    \retval     none 
*/
void dma_periph_address_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t address)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHPADDR(dma_periph, channelx) = address;
}

/*!
    \brief      set DMA memory base address  
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set memory base address 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  address: memory base address
    \param[out] none
    \retval     none 
*/
void dma_memory_address_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t address)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHMADDR(dma_periph, channelx) = address;
}

/*!
    \brief      set the number of remaining data to be transferred by the DMA 
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set number 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  number: the number of remaining data to be transferred by the DMA
    \param[out] none
    \retval     none 
*/
void dma_transfer_number_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t number)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCNT(dma_periph, channelx) = (number & DMA_CHANNEL_CNT_MASK);
}

/*!
    \brief      get the number of remaining data to be transferred by the DMA  
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set number 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     uint32_t: the number of remaining data to be transferred by the DMA 
*/
uint32_t dma_transfer_number_get(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    return (uint32_t)DMA_CHCNT(dma_periph, channelx);
}

/*!
    \brief      configure priority level of DMA channel 
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  priority: priority Level of this channel
      \arg        DMA_PRIORITY_LOW: low priority
      \arg        DMA_PRIORITY_MEDIUM: medium priority
      \arg        DMA_PRIORITY_HIGH: high priority
      \arg        DMA_PRIORITY_ULTRA_HIGH: ultra high priority
    \param[out] none
    \retval     none 
*/
void dma_priority_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t priority)
{
    uint32_t ctl;
    
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_PRIO;
    ctl |= priority;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    \brief      configure transfer data size of memory 
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  mwidth: transfer data width of memory
      \arg        DMA_MEMORY_WIDTH_8BIT: transfer data width of memory is 8-bit
      \arg        DMA_MEMORY_WIDTH_16BIT: transfer data width of memory is 16-bit
      \arg        DMA_MEMORY_WIDTH_32BIT: transfer data width of memory is 32-bit
    \param[out] none
    \retval     none
*/
void dma_memory_width_config (uint32_t dma_periph, dma_channel_enum channelx, uint32_t mwidth)
{
    uint32_t ctl;
    
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_MWIDTH;
    ctl |= mwidth;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    \brief      configure transfer data size of peripheral 
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  pwidth: transfer data width of peripheral
      \arg        DMA_PERIPHERAL_WIDTH_8BIT: transfer data width of peripheral is 8-bit
      \arg        DMA_PERIPHERAL_WIDTH_16BIT: transfer data width of peripheral is 16-bit
      \arg        DMA_PERIPHERAL_WIDTH_32BIT: transfer data width of peripheral is 32-bit
    \param[out] none
    \retval     none
*/
void dma_periph_width_config (uint32_t dma_periph, dma_channel_enum channelx, uint32_t pwidth)
{
    uint32_t ctl;
    
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_PWIDTH;
    ctl |= pwidth;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    \brief      enable next address increasement algorithm of memory  
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     none
*/
void dma_memory_increase_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MNAGA;
}

/*!
    \brief      disable next address increasement algorithm of memory  
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     none
*/
void dma_memory_increase_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MNAGA;
}

/*!
    \brief      enable next address increasement algorithm of peripheral
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     none
*/
void dma_periph_increase_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
}

/*!
    \brief      disable next address increasement algorithm of peripheral 
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[out] none
    \retval     none
*/
void dma_periph_increase_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_PNAGA;
}

/*!
    \brief      configure the direction of data transfer on the channel  
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  direction: specify the direction of data transfer
      \arg        DMA_PERIPHERAL_TO_MEMORY: read from peripheral and write to memory
      \arg        DMA_MEMORY_TO_PERIPHERAL: read from memory and write to peripheral
    \param[out] none
    \retval     none
*/
void dma_transfer_direction_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t direction)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    if(DMA_PERIPHERAL_TO_MEMORY == direction){
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_DIR;
    } else {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_DIR;
    }
}

/*!
    \brief      check DMA flag is set or not 
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to get flag
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  flag: specify get which flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_FLAG_G: global interrupt flag of channel
      \arg        DMA_FLAG_FTF: full transfer finish flag of channel
      \arg        DMA_FLAG_HTF: half transfer finish flag of channel
      \arg        DMA_FLAG_ERR: error flag of channel
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus dma_flag_get(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag)
{
    FlagStatus reval;

    if(RESET != (DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx))){
        reval = SET;
    }else{
        reval = RESET;
    }
    
    return reval;
}

/*!
    \brief      clear DMA a channel flag
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to clear flag
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  flag: specify get which flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_FLAG_G: global interrupt flag of channel
      \arg        DMA_FLAG_FTF: full transfer finish flag of channel
      \arg        DMA_FLAG_HTF: half transfer finish flag of channel
      \arg        DMA_FLAG_ERR: error flag of channel
    \param[out] none
    \retval     none
*/
void dma_flag_clear(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag)
{
    DMA_INTC(dma_periph) |= DMA_FLAG_ADD(flag, channelx);
}

/*!
    \brief      check DMA flag and interrupt enable bit is set or not 
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to get flag
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  flag: specify get which flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_INT_FLAG_FTF: full transfer finish interrupt flag of channel
      \arg        DMA_INT_FLAG_HTF: half transfer finish interrupt flag of channel
      \arg        DMA_INT_FLAG_ERR: error interrupt flag of channel
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus dma_interrupt_flag_get(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag)
{
    uint32_t interrupt_enable = 0U, interrupt_flag = 0U;
    
    switch(flag){
        case DMA_INT_FLAG_FTF:
            interrupt_flag = DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_FTFIE;
            break;
        case DMA_INT_FLAG_HTF:
            interrupt_flag = DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_HTFIE;
            break;
        case DMA_INT_FLAG_ERR:
            interrupt_flag = DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_ERRIE;
            break;
        default:
            DMA_WRONG_HANDLE
        }
    
    if(interrupt_flag && interrupt_enable){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    \brief      clear DMA a channel flag
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to clear flag
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  flag: specify get which flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_INT_FLAG_GIF: global interrupt flag of channel
      \arg        DMA_INT_FLAG_FTFIF: full transfer finish interrupt flag of channel
      \arg        DMA_INT_FLAG_HTFIF: half transfer finish interrupt flag of channel
      \arg        DMA_INT_FLAG_ERRIF: error interrupt flag of channel
    \param[out] none
    \retval     none
*/
void dma_interrupt_flag_clear(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag)
{
    DMA_INTC(dma_periph) |= DMA_FLAG_ADD(flag, channelx);
}

/*!
    \brief      enable DMA interrupt
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  source: specify which interrupt to enbale
                one or more parameters can be selected which are shown as below
      \arg        DMA_INT_FTFIE: channel full transfer finish interrupt
      \arg        DMA_INT_HTFIE: channel half transfer finish interrupt
      \arg        DMA_INT_ERRIE: channel error interrupt
    \param[out] none
    \retval     none
*/
void dma_interrupt_enable(uint32_t dma_periph, dma_channel_enum channelx, uint32_t source)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) |= source;
}

/*!
    \brief      disable DMA interrupt
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    \param[in]  source: specify which interrupt to disbale
                one or more parameters can be selected which are shown as below
      \arg        DMA_INT_FTFIE: channel full transfer finish interrupt
      \arg        DMA_INT_HTFIE: channel half transfer finish interrupt
      \arg        DMA_INT_ERRIE: channel error interrupt
    \param[out] none
    \retval     none
*/
void dma_interrupt_disable(uint32_t dma_periph, dma_channel_enum channelx, uint32_t source)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) &= ~source;
}

/*!
    \brief      check whether peripheral and channels match
    \param[in]  dma_periph: DMAx(x=0,1)
      \arg        DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel 
      \arg        DMA_CHx(x=0..6)
    \param[out] none
    \retval     none
*/
static ErrStatus dma_periph_and_channel_check(uint32_t dma_periph, dma_channel_enum channelx)
{
    ErrStatus val = SUCCESS;
    
    if(DMA1 == dma_periph){
        if(channelx > DMA_CH4){
            val = ERROR;
        }
    }
    
    return val;
}

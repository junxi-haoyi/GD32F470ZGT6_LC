/*!
    \file  gd32f403_rtc.c
    \brief RTC driver
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-02-10, V1.0.1, firmware for GD32F403
*/

#include "gd32f403_rtc.h"

/*!
    \brief      enable RTC interrupt
    \param[in]  interrupt: specify which interrupt to enbale
          \arg    RTC_INT_SECOND: second interrupt
          \arg    RTC_INT_ALARM: alarm interrupt
          \arg    RTC_INT_OVERFLOW: overflow interrupt
    \param[out] none
    \retval     none
*/
void rtc_interrupt_enable(uint32_t interrupt)
{
    RTC_INTEN |= interrupt;
}

/*!
    \brief      disable RTC interrupt
    \param[in]  interrupt: specify which interrupt to disbale
          \arg    RTC_INT_SECOND: second interrupt
          \arg    RTC_INT_ALARM: alarm interrupt
          \arg    RTC_INT_OVERFLOW: overflow interrupt
    \param[out] none
    \retval     none
*/
void rtc_interrupt_disable(uint32_t interrupt)
{
    RTC_INTEN &= ~interrupt;
}

/*!
    \brief      enter RTC configuration mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_configuration_mode_enter(void)
{
    RTC_CTL |= RTC_CTL_CMF;
}

/*!
    \brief      exit RTC configuration mode 
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_configuration_mode_exit(void)
{
    RTC_CTL &= ~RTC_CTL_CMF;
}

/*!
    \brief      wait RTC last write operation finished flag set
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_lwoff_wait(void)
{
     /* loop until LWOFF flag is set */
    while (RESET == (RTC_CTL & RTC_CTL_LWOFF)){
    }
}

/*!
    \brief      wait RTC registers synchronized flag set
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_register_sync_wait(void)
{
     /* clear RSYNF flag */
    RTC_CTL &= ~RTC_CTL_RSYNF;
    /* loop until RSYNF flag is set */
    while (RESET == (RTC_CTL & RTC_CTL_RSYNF)){
    }
}

/*!
    \brief      get RTC counter value
    \param[in]  none
    \param[out] none
    \retval     RTC counter value
*/
uint32_t rtc_counter_get(void)
{
    uint32_t temp = 0x0U;
    temp = RTC_CNTL;
    temp |= (RTC_CNTH << 16);
    return temp;
}

/*!
    \brief      set RTC counter value
    \param[in]  cnt: RTC counter value
    \param[out] none
    \retval     none
*/
void rtc_counter_set(uint32_t cnt)
{
    rtc_configuration_mode_enter();
    /* set the RTC counter high bits */
    RTC_CNTH = cnt >> 16;
    /* set the RTC counter low bits */
    RTC_CNTL = (cnt & RTC_LOW_VALUE);
    rtc_configuration_mode_exit();
}

/*!
    \brief      set RTC prescaler value
    \param[in]  psc: RTC prescaler value
    \param[out] none
    \retval     none
*/
void rtc_prescaler_set(uint32_t psc)
{
    rtc_configuration_mode_enter();
    /* set the RTC prescaler high bits */
    RTC_PSCH = (psc & RTC_HIGH_VALUE) >> 16;
    /* set the RTC prescaler low bits */
    RTC_PSCL = (psc & RTC_LOW_VALUE);
    rtc_configuration_mode_exit();
}

/*!
    \brief      set RTC alarm value
    \param[in]  alarm: RTC alarm value
    \param[out] none
    \retval     none
*/
void rtc_alarm_config(uint32_t alarm)
{
    rtc_configuration_mode_enter();
    /* set the alarm high bits */
    RTC_ALRMH = alarm >> 16;
    /* set the alarm low bits */
    RTC_ALRML = (alarm & RTC_LOW_VALUE);
    rtc_configuration_mode_exit();
}

/*!
    \brief      get RTC divider value
    \param[in]  none
    \param[out] none
    \retval     RTC divider value
*/
uint32_t rtc_divider_get(void)
{
    uint32_t temp = 0x00U;
    temp = (RTC_DIVH & RTC_DIVH_DIV) << 16;
    temp |= RTC_DIVL;
    return temp;
}

/*!
    \brief      get RTC flag status 
    \param[in]  flag: specify which flag status to get
          \arg    RTC_FLAG_SECOND: second interrupt flag
          \arg    RTC_FLAG_ALARM: alarm interrupt flag
          \arg    RTC_FLAG_OVERFLOW: overflow interrupt flag
          \arg    RTC_FLAG_RSYN: registers synchronized flag
          \arg    RTC_FLAG_LWOF: last write operation finished flag
    \param[out] none
    \retval     SET or RESET
*/
FlagStatus rtc_flag_get(uint32_t flag)
{
    if(RESET != (RTC_CTL & flag)){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    \brief      clear RTC flag status
    \param[in]  flag: specify which flag status to clear
          \arg    RTC_FLAG_SECOND: second interrupt flag
          \arg    RTC_FLAG_ALARM: alarm interrupt flag
          \arg    RTC_FLAG_OVERFLOW: overflow interrupt flag
          \arg    RTC_FLAG_RSYN: registers synchronized flag
    \param[out] none
    \retval     none
*/
void rtc_flag_clear(uint32_t flag)
{
    /* clear RTC flag */
    RTC_CTL &= ~flag;
}


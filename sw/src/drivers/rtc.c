/*
 * Copyright (c) 2019 Jakub Kaderka
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/cm3/nvic.h>

#include "drivers/rtc.h"

void rtc_isr(void)
{
    rtc_clear_wakeup_flag();

    rtc_unlock();

    if (RTC_ISR & RTC_ISR_ALRAF) {
        exti_reset_request(EXTI17);
        RTC_CR &= ~(RTC_CR_ALRAIE | RTC_CR_ALRAE);
        RTC_ISR &= ~RTC_ISR_ALRAF;
    }

    /* WUTF flag seems to be not set correctly, erase wut every time */
    exti_reset_request(EXTI20);
    RTC_CR &= ~(RTC_CR_WUTIE | RTC_CR_WUTE);
    RTC_ISR &= ~RTC_ISR_WUTF;

    rtc_lock();

    nvic_disable_irq(NVIC_RTC_IRQ);
    printf("woken\n");
}

void RTCd_SetWakeup(uint32_t period_s)
{
    rtc_unlock();

    rtc_set_wakeup_time(period_s, RTC_CR_WUCLKSEL_SPRE);
    RTC_CR |= RTC_CR_WUTIE;

    rtc_lock();

    exti_set_trigger(EXTI20, EXTI_TRIGGER_RISING);
    exti_enable_request(EXTI20);

    nvic_enable_irq(NVIC_RTC_IRQ);
}

void RTCd_SetAlarm(uint32_t alarm_in_s)
{
    uint32_t time;
    uint8_t sec;

    time = RTC_TR;
    sec = ((time >> RTC_TR_ST_SHIFT) & RTC_TR_ST_MASK) * 10;
    sec += ((time >> RTC_TR_SU_SHIFT) & RTC_TR_SU_MASK);
    sec += alarm_in_s;
    sec = sec % 60;

    rtc_unlock();

    RTC_CR &= ~RTC_CR_ALRAE;

    /* Wait until we can write */
    while ((RTC_ISR & RTC_ISR_ALRAWF) == 0) {
        ;
    }

    /* enable seconds alarm */
    RTC_ALRMAR = ~(RTC_ALRMXR_MSK1 | (RTC_ALRMXR_ST_MASK << RTC_ALRMXR_ST_SHIFT) | (RTC_ALRMXR_SU_MASK << RTC_ALRMXR_SU_SHIFT));
    RTC_ALRMAR |= (sec / 10) << RTC_ALRMXR_ST_SHIFT;
    RTC_ALRMAR |= (sec % 10) << RTC_ALRMXR_SU_SHIFT;

    RTC_CR |= RTC_CR_ALRAE | RTC_CR_ALRAIE;

    rtc_lock();

    exti_set_trigger(EXTI17, EXTI_TRIGGER_RISING);
    exti_enable_request(EXTI17);

    nvic_enable_irq(NVIC_RTC_IRQ);
}

void RTCd_Init(void)
{
    rcc_periph_clock_enable(RCC_PWR);
    rcc_periph_clock_enable(RCC_RTC);
    pwr_disable_backup_domain_write_protect();

    rcc_osc_on(RCC_LSI);
    rcc_wait_for_osc_ready(RCC_LSI);
    rcc_set_rtc_clock_source(RCC_LSI);
    rcc_enable_rtc_clock();

    rtc_unlock();

    RTC_ISR |= RTC_ISR_INIT;
    while ((RTC_ISR & RTC_ISR_INITF) == 0){
        ;
    }

    /* Clear config register */
    RTC_CR = 0;
    RTC_ISR = 0;

    /* set synch prescaler, using defaults for 1Hz out */
    uint32_t sync = 255;
    uint32_t async = 127;
    rtc_set_prescaler(sync, async);

    RTC_ISR &= ~(RTC_ISR_INIT);

    rtc_lock();

    RCC_BDCR |= RCC_BDCR_RTCEN;
    rtc_wait_for_synchro();
}

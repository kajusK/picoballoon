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
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/dbgmcu.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>
#include "drivers/rtc.h"
#include "drivers/wdg.h"
#include "drivers/systick.h"

#include "drivers/power.h"

#define PORT_LATCH GPIOB
#define PIN_LATCH GPIO8
#define RCC_LATCH RCC_GPIOB

#define SLEEP_TIME_S ((WDG_PERIOD_MS * 9) / 10)

void Powerd_ShutDown(void)
{
    rcc_periph_clock_enable(RCC_LATCH);
    gpio_mode_setup(PORT_LATCH, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, PIN_LATCH);
    gpio_clear(PORT_LATCH, PIN_LATCH);

    /* Should be dead already, or at least should die very soon, if not, wdg
     * will do the rest */
    while (1) {
        ;
    }
}

void Powerd_Sleep(uint32_t time_s)
{
    uint32_t sec_passed = 0;

    rcc_periph_clock_enable(RCC_PWR);

    pwr_set_stop_mode();
    pwr_voltage_regulator_low_power_in_stop();
    DBGMCU_CR &= ~DBGMCU_CR_STOP;
    SCB_SCR |= SCB_SCR_SLEEPDEEP | SCB_SCR_SLEEPONEXIT;

    /* Unable to sleep for longer than wdg period */
    while (sec_passed < time_s) {
        uint32_t sleep_period;
        if (time_s - sec_passed > SLEEP_TIME_S) {
            sleep_period = SLEEP_TIME_S;
        } else {
            sleep_period = time_s - sec_passed;
        }

        RTCd_SetWakeup(sleep_period);
        __asm volatile ("wfi");
        Wdgd_Clear();
        sec_passed += sleep_period;
    }
}

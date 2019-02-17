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

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/exti.h>

#include "lmic/oslmic.h"
#include "lmic/hal.h"
#include "lmic.h"

#define PORT_NSS GPIOB
#define PIN_NSS GPIO6
#define RCC_NSS RCC_GPIOB

#define PORT_RST GPIOB
#define PIN_RST GPIO7
#define RCC_RST RCC_GPIOB

#define EXT_DIO0 8
#define EXT_DIO1 15

/* EXT8 (PA8) is DIO0, EXT15 (PA15) DIO1 */
void exti4_15_isr (void)
{
    if (exti_get_flag_status(EXT_DIO0)) {
        exti_reset_request(EXT_DIO0);
        radio_irq_handler(0);
    } else if (exti_get_flag_status(EXT_DIO1)) {
        exti_reset_request(EXT_DIO1);
        radio_irq_handler(1);
    }
}

/*
 * initialize hardware (IO, SPI, TIMER, IRQ).
 */
void hal_init(void)
{
    rcc_periph_clock_enable(RCC_NSS);
    rcc_periph_clock_enable(RCC_RST);

    gpio_mode_setup(PORT_NSS, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_NSS);
    gpio_mode_setup(PORT_RST, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_RST);

    /* interrupts for DIO */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_SYSCFG_COMP);
    exti_select_source(EXT_DIO0, GPIOA);
    exti_select_source(EXT_DIO1, GPIOA);
    exti_set_trigger(EXT_DIO0, EXTI_TRIGGER_RISING);
    exti_set_trigger(EXT_DIO1, EXTI_TRIGGER_RISING);
    exti_enable_request(EXT_DIO0);
    exti_enable_request(EXT_DIO1);
    hal_enableIRQs();

    /* timer 2 as os tick - 32 bit, period 16 us */
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_reset_pulse(RST_TIM2);
    /* frequency to 62,5 kHz - 16 us tick */
    timer_set_prescaler(TIM2, ((rcc_apb1_frequency) / 62500));
    timer_disable_preload(TIM2);
    timer_continuous_mode(TIM2);
    timer_set_period(TIM2, (uint32_t) -1);
    timer_enable_counter(TIM2);

    /* Configure SPI GPIOs: SCK=PA5, MISO=PA6 and MOSI=PA7 */
    rcc_periph_clock_enable(RCC_SPI1);
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE,  GPIO3 | GPIO4 | GPIO5);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, GPIO5 | GPIO3);
    gpio_set_af(GPIOB, GPIO_AF0, GPIO3 | GPIO4 | GPIO5);

    spi_reset(SPI1);
    spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_4, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_LSBFIRST);
    spi_set_full_duplex_mode(SPI1);

    /* Ignore the stupid NSS pin. This is required for spi to work */
    spi_enable_software_slave_management(SPI1);
    spi_set_nss_high(SPI1);
    spi_disable_crc(SPI1);

    spi_enable(SPI1);
}

/*
 * drive radio NSS pin (0=low, 1=high).
 */
void hal_pin_nss(u1_t val)
{
    if (val) {
        gpio_set(PORT_NSS, PIN_NSS);
    } else {
        gpio_clear(PORT_NSS, PIN_NSS);
    }
}

/*
 * drive radio RX/TX pins (0=rx, 1=tx).
 */
void hal_pin_rxtx(u1_t val)
{
    (void) val;
    /* RXTX used for switching antenna for RX and TX on some boards */
}

/*
 * control radio RST pin (0=low, 1=high, 2=floating)
 */
void hal_pin_rst(u1_t val)
{
    if (val == 1) {
        gpio_set(PORT_RST, PIN_RST);
    } else if (val == 0) {
        gpio_clear(PORT_RST, PIN_RST);
    } else {
        gpio_mode_setup(PORT_RST, GPIO_MODE_INPUT, GPIO_PUPD_NONE, PIN_RST);
    }
}

/*
 * perform 8-bit SPI transaction with radio.
 *   - write given byte 'outval'
 *   - read byte and return value
 */
u1_t hal_spi(u1_t outval)
{
    return spi_xfer(SPI1, outval);
}

/*
 * disable all CPU interrupts.
 *   - might be invoked nested
 *   - will be followed by matching call to hal_enableIRQs()
 */
void hal_disableIRQs(void)
{
    nvic_disable_irq(NVIC_EXTI4_15_IRQ);
}

/*
 * enable CPU interrupts.
 */
void hal_enableIRQs(void)
{
    nvic_enable_irq(NVIC_EXTI4_15_IRQ);
}

/*
 * put system and CPU in low-power mode, sleep until interrupt.
 */
void hal_sleep(void)
{
    //not implemented
}

/*
 * return 32-bit system time in ticks.
 */
u4_t hal_ticks(void)
{
    return  timer_get_counter(TIM2);
}

/*
 * busy-wait until specified timestamp (in ticks) is reached.
 */
void hal_waitUntil(u4_t time)
{
    while (timer_get_counter(TIM2) < time) {
        ;
    }
}

/*
 * check and rewind timer for target time.
 *   - return 1 if target time is close
 *   - otherwise rewind timer for target time or full period and return 0
 */
u1_t hal_checkTimer(u4_t targettime)
{
    return timer_get_counter(TIM2) >= targettime;
}

/*
 * perform fatal failure action.
 *   - called by assertions
 *   - action could be HALT or reboot
 */
void hal_failed(const char *file, u2_t line)
{
    (void) file;
    (void) line;
    while (1) {
        ;
    }
}


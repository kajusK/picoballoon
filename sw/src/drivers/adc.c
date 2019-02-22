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
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dma.h>

#include "drivers/adc.h"

#define ADC_TEMP_CHANNEL    16
#define ADC_INT_REF_CHANNEL 17

/* Resolution of adc is 12 bits by default - 2^12 */
#define ADC_MAX 4095

/** average temperature sensor slope in 3,3 V/°C multiplied by 1000 */
#define TEMP_SLOPE 5336
/** Calibration value for internal temperature sensor at 30 deg C */
#define TEMP30_CAL (*((uint16_t*) ((uint32_t) 0x1FFFF7B8)))
/** Calibration value for internal reference */
#define VREFINT_CAL (*((uint16_t *) ((uint32_t) 0x1FFFF7BA)))

static uint16_t adcdi_vdda_mv = 3300;

void Adcd_SetAnalog(uint32_t port, uint16_t gpios)
{
    gpio_mode_setup(port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, gpios);
}

uint16_t Adcd_ReadRaw(uint8_t channel)
{
    adc_set_regular_sequence(ADC1, 1, &channel);
    adc_start_conversion_regular(ADC1);
    while (!adc_eoc(ADC1)) {
        ;
    }

    return adc_read_regular(ADC1);
}

uint16_t Adcd_ReadMv(uint8_t channel)
{
    uint16_t raw = Adcd_ReadRaw(channel);

    return (uint32_t) adcdi_vdda_mv * raw / ADC_MAX;
}

uint16_t Adcd_ReadVccMv(void)
{
    uint16_t raw = Adcd_ReadRaw(ADC_INT_REF_CHANNEL);
    return 3300U * VREFINT_CAL/raw;
}

int16_t Adcd_ReadTempDegC(void)
{
    uint16_t raw = Adcd_ReadRaw(ADC_TEMP_CHANNEL);
    uint16_t ref_mv = adcdi_vdda_mv;
    int32_t temp;

    temp = ((uint32_t) TEMP30_CAL - ((uint32_t) raw * ref_mv / 3300))*1000;
    temp = (temp / TEMP_SLOPE) + 30;
    return temp;
}

void Adcd_UpdateVdda(void)
{
    adcdi_vdda_mv = Adcd_ReadVccMv();
}

void Adcd_Init(void)
{
    int i;

    rcc_periph_clock_enable(RCC_ADC);

    adc_power_off(ADC1);
    adc_enable_temperature_sensor();
    adc_enable_vrefint();
    adc_set_clk_source(ADC1, ADC_CLKSOURCE_ADC);
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPTIME_071DOT5);
    adc_set_resolution(ADC1, ADC_RESOLUTION_12BIT);
    adc_set_operation_mode(ADC1, ADC_MODE_SCAN);
    adc_disable_external_trigger_regular(ADC1);
    adc_set_right_aligned(ADC1);
    adc_disable_analog_watchdog(ADC1);
    adc_calibrate(ADC1);
    adc_power_on(ADC1);

    /* Wait for ADC starting up. */
    for (i = 0; i < 800000; i++) {
        __asm__("nop");
    }

    Adcd_UpdateVdda();
}

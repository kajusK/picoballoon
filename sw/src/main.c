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

#include <inttypes.h>
#include <stdio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "drivers/wdg.h"
#include "drivers/systick.h"
#include "drivers/gps.h"
#include "drivers/adc.h"
#include "drivers/stdout.h"
#include "drivers/i2c.h"
#include "drivers/power.h"
#include "drivers/rtc.h"
#include "gps.h"
#include "sensors.h"
#include "lora.h"
#include "probe.h"
#include "main.h"

/**
 * Print out debug message containing all the sensors data
 */
static void Print_Debug(void)
{
    struct minmea_sentence_gga *frame;
    uint16_t cap_mv = Sensors_GetCapMv();
    uint16_t solar_mv = Sensors_GetSolarMv();
    uint16_t core_mv = Adcd_ReadVccMv();
    int16_t core_c = Adcd_ReadTempDegC();
    int8_t temp2 = Sensors_TempDegC();
    uint32_t pressure;
    int32_t temp;

    Sensors_PressurecMbar(&pressure, &temp);

    puts("\n\n-----------\nProbe debug:");
    printf("Capacitor: %d mV\n", cap_mv);
    printf("Solar cell: %d mV\n", solar_mv);
    printf("Core voltage: %d mV\n", core_mv);
    printf("Core temperature: %d C\n", core_c);
    printf("Pressure: %ld mbar\n", pressure / 100);
    printf("Temperature: %ld C\n", temp / 100);
    printf("Temperature sensor: %d C\n", temp2);

    if (!Gps_GotFix()) {
        puts("GPS fix: No\n----------");
        return;
    }

    frame = Gps_GetGga();
    printf("GPS fix: coordinates lat %f lon %f altitude %d\n",
        minmea_tocoord(&frame->latitude),
        minmea_tocoord(&frame->longitude),
        (int) (frame->altitude.value / frame->altitude.scale));
    puts("-------------\n");
}

/**
 * Set all gpio as inputs with pullup
 *
 * Avoids floating pins to reduce power consumption
 */
static void Sysi_GpioDefault(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOF);
    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, 0xffff);
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, 0xffff);
    gpio_mode_setup(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, 0xffff);
    gpio_mode_setup(GPIOF, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, 0xffff);
}

static void Sysi_Init(void)
{
    Sysi_GpioDefault();

    //Wdgd_Init();
    Stdoutd_Init();
    Systickd_Init();

    Adcd_Init();
    I2Cd_Init();
//    RTCd_Init();

    GPSd_Init();
    Sensors_PinsInit();
}

void Sys_Sleep(uint32_t sleep_s)
{
    puts("Going to sleep. Good night.");
    while (!Lora_IsAllSent()) {
        Lora_Update();
    }
    GPSd_SetPower(false);
    Lora_PowerOff();
    Adcd_Sleep();
    Wdgd_Clear();

    Powerd_Sleep(sleep_s*60*1000U);

    /** Reinitialize all peripherals after sleep mode */
    Sysi_Init();
    Wdgd_Clear();
    Gps_FixClear();
    Adcd_Wakeup();
    Lora_PowerOn();
    GPSd_SetPower(true);
}

void Sys_Shutdown(void)
{
    puts("Shutting down... Don't want do die yet, I'm too young!");
    /* Wait for lora to send all remaining data */
    while (!Lora_IsAllSent()) {
        Lora_Update();
    }
    Powerd_ShutDown();
}

int main(void)
{
    Sysi_Init();

    puts("Brno Observatory and Planetarium Pico Balloon Challange 2019");
    puts("Team DeadBadger, Jakub Kaderka 2019");
    puts("-------------------------");

    GPSd_SetPowerSave();
    GPSd_SetPower(true);
    Sensors_PressureInit();
    Lora_Init();

    while (1) {
        Lora_Update();
        //Print_Debug()
        Adcd_UpdateVdda();
        Probe_Loop();
        Wdgd_Clear();
    }
}

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

#include <stdlib.h>
#include "drivers/systick.h"
#include "drivers/adc.h"
#include "drivers/i2c.h"

#include "sensors.h"

#define AD_CHANNEL_SUPERCAP 0
#define AD_CHANNEL_SOLAR 1
#define AD_CHANNEL_TEMP 5

/** Time for pressure sensor conversion */
#define MS5607_CONVERT_MS 10
/** I2C address of the pressure sensor */
#define MS5607_ADDR 0x76

/** Pressure sensor commands */
#define MS5607_CMD_RESET 0x1e
#define MS5607_CMD_PROM 0xa0
#define MS5607_CMD_READ_ADC 0x00
#define MS5607_CMD_CONV_PRESS 0x48
#define MS5607_CMD_CONV_TEMP 0x58

static const int16_t lmt87_lookup[][2] = {
    {3277, -50},
    {3160, -40},
    {3030, -30},
    {2899, -20},
    {2767, -10},
    {2633, 0},
    {2365, 20},
    {2095, 40},
    {1819, 60},
    {1539, 80},
    {1257, 100},
};

static uint16_t ms5607_calib[6];

static void Sensorsi_MS5607Cmd(uint8_t cmd, uint8_t *buf, uint8_t len)
{
    I2Cd_Transceive(MS5607_ADDR, &cmd, 1, buf, len);
}

int32_t Sensors_TempDegC(void)
{
    int i;
    int16_t x1, x2;
    int16_t y1, y2;
    uint8_t size = sizeof(lmt87_lookup)/sizeof(lmt87_lookup[0]);
    uint16_t volt = Adcd_ReadMv(AD_CHANNEL_TEMP);

    for (i = 0; i < size; i++) {
        if (volt >= lmt87_lookup[i][0]) {
            break;
        }
    }

    if (i == 0) {
        i = 1;
    } else if (i == size) {
        i = size - 1;
    }

    x1 = lmt87_lookup[i-1][0];
    x2 = lmt87_lookup[i][0];
    y1 = lmt87_lookup[i-1][1];
    y2 = lmt87_lookup[i][1];

    return y1 + ((int32_t)(volt - x1)*(y2-y1))/(x2 - x1);
}

void Sensors_PressurecMbar(uint32_t *pressure, int32_t *temperature)
{
    uint32_t d2;
    uint32_t d1;
    int32_t temp, t2, dt;
    int64_t off, off2, sens, sens2;
    int32_t press;

    Sensorsi_MS5607Cmd(MS5607_CMD_CONV_TEMP, NULL, 0);
    delay_ms(MS5607_CONVERT_MS);
    Sensorsi_MS5607Cmd(MS5607_CMD_READ_ADC, (uint8_t *) &d2, 3);

    Sensorsi_MS5607Cmd(MS5607_CMD_CONV_PRESS, NULL, 0);
    delay_ms(MS5607_CONVERT_MS);
    Sensorsi_MS5607Cmd(MS5607_CMD_READ_ADC, (uint8_t *) &d1, 3);

    dt = d2 - (uint32_t)(ms5607_calib[4] << 8);
    temp = 2000 + (((int64_t) dt*ms5607_calib[5]) >> 23);

    /* Second order compensation */
    if (temp >= 2000) {
        t2 = 0;
        sens2 = 0;
        off2 = 0;
    } else {
        int64_t square = (temp-2000);
        square *= square;

        t2 = ((int64_t) dt * dt) >> 31;
        off2 = 61*(square >> 4);
        sens2 = 2*square;
        if (temp < -1500) {
            square = temp + 1500;
            square *= square;

            off2 += 15*square;
            sens2 += 8*square;
        }
    }

    off = ((int64_t)ms5607_calib[1] << 17) + (((int64_t)ms5607_calib[3]*dt) >> 6);
    sens = ((int64_t)ms5607_calib[0] << 16) + (((int64_t)ms5607_calib[2]*dt) >> 7);

    temp = temp - t2;
    off = off - off2;
    sens = sens - sens2;
    press = (((d1*sens) >> 21) - off) >> 15;

    *pressure = press;
    *temperature = temp;
}

uint16_t Sensors_GetSolarMv(void)
{
    return Adcd_ReadMv(AD_CHANNEL_SOLAR);
}

uint16_t Sensors_GetCapMv(void)
{
    return Adcd_ReadMv(AD_CHANNEL_SUPERCAP);
}

void Sensors_Init(void)
{
    Adcd_SetAnalog(GPIOA, 1 << AD_CHANNEL_SUPERCAP);
    Adcd_SetAnalog(GPIOA, 1 << AD_CHANNEL_SOLAR);
    Adcd_SetAnalog(GPIOA, 1 << AD_CHANNEL_TEMP);

    /** Read calibration data for pressure calculations */
    for (int i = 0; i < 6; i++) {
        Sensorsi_MS5607Cmd(MS5607_CMD_PROM + (i << 1),
                (uint8_t *) &ms5607_calib[i], 2);
    }
}

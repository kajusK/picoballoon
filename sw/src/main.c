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
#include <inttypes.h>
#include <stdio.h>

#include "drivers/wdg.h"
#include "drivers/systick.h"
#include "drivers/gps.h"
#include "drivers/adc.h"
#include "drivers/stdout.h"
#include "drivers/i2c.h"
#include "gps.h"
#include "sensors.h"
#include "lora.h"

void data_sender(void)
{
    Lora_Send("foo", 3);

    //printf("Sending pos lat %d°%d.%d %c lon %d°%d.%d %c", );
}

int main(void)
{
    rcc_clock_setup_in_hsi_out_48mhz();
    Wdgd_Init();
    Systickd_Init();
    Stdoutd_Init();
    Adcd_Init();
    GPSd_Init();
    I2Cd_Init();
    Sensors_Init();
    Lora_Init();

    /* wait for gps fix */
    while (GPS_get_data() == NULL) {
        ;
    }

    while (1) {
        Lora_Update();
        Wdgd_Clear();
    }
}

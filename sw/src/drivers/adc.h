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

#ifndef __DRIVERS_ADC_H_
#define __DRIVERS_ADC_H_

#include <inttypes.h>

/**
 * Set pin as analog input
 */
extern void Adcd_SetAnalog(uint32_t port, uint16_t gpios);

/**
 * Read the raw 12 bit data from adc conversion
 */
extern uint16_t Adcd_ReadRaw(uint8_t channel);

/**
 * Read voltage on the channel in mV
 */
extern uint16_t Adcd_ReadMv(uint8_t channel);

/**
 * Read power supply voltage
 */
extern uint16_t Adcd_ReadVccMv(void);

/**
 * Read core temeperature in degrees C
 */
extern int16_t Adcd_ReadTempDegC(void);

/**
 * Update reference voltage from internal reference measurements
 *
 * Should be called from time to time in vdda is not very stable
 */
extern void Adcd_UpdateVdda(void);

/**
 * Initialize the adc driver
 */
extern void Adcd_Init(void);

#endif

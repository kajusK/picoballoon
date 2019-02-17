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

#ifndef __SENSORS_H
#define __SENSORS_H

#include <inttypes.h>
#include <stdbool.h>

/**
 * Get ambient temperature in degrees C
 */
extern int32_t Sensors_TempDegC(void);

/**
 * Get ambient air pressure in centi mbar (110002 = 1100.02 mBar)
 */
extern void Sensors_PressurecMbar(uint32_t *pressure, int32_t *temperature);

/** Voltage on solar array */
extern uint16_t Sensors_GetSolarMv(void);

/** Voltage on power capacitor */
extern uint16_t Sensors_GetCapMv(void);

/**
 * Initialize sensors
 */
extern void Sensors_Init(void);

#endif

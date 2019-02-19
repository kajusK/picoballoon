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

#ifndef __DRIVERS_I2C_H
#define __DRIVERS_I2C_H

/**
 * Send/receive data over i2c
 *
 * @param [in] address      Device address (7 bit)
 * @param [in] txbuf        Data to send or NULL
 * @param [in] txlen        Length of txbuf
 * @param [out] rxbuf       Buffer for received data or NULL
 * @param [in] rxlen        Amount of bytes to receive
 */
extern bool I2Cd_Transceive(uint8_t address, uint8_t *txbuf, uint8_t txlen,
        uint8_t *rxbuf, uint8_t rxlen);

/**
 * Initialize the i2c peripheral
 */
extern void I2Cd_Init(void);

#endif

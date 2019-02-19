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
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>

#include "drivers/i2c.h"

bool I2Cd_Transceive(uint8_t address, uint8_t *txbuf, uint8_t txlen,
        uint8_t *rxbuf, uint8_t rxlen)
{
    uint32_t i2c = I2C2;

    /** modified code from libopencm3 library - infinite loop on nack, wtf?? */
    /*  waiting for busy is unnecessary. read the RM */
    if (txlen) {
        i2c_set_7bit_address(i2c, address);
        i2c_set_write_transfer_dir(i2c);
        i2c_set_bytes_to_transfer(i2c, txlen);
        if (rxlen) {
            i2c_disable_autoend(i2c);
        } else {
            i2c_enable_autoend(i2c);
        }
        i2c_send_start(i2c);

        while (txlen--) {
            bool wait = true;
            while (wait) {
                if (i2c_transmit_int_status(i2c)) {
                    wait = false;
                }
                if (i2c_nack(i2c)) {
                    return false;
                }
            }
            i2c_send_data(i2c, *txbuf++);
        }
        /* not entirely sure this is really necessary.
         * RM implies it will stall until it can write out the later bits
         */
        if (rxlen) {
            while (!i2c_transfer_complete(i2c));
        }
    }

    if (rxlen) {
        /* Setting transfer properties */
        i2c_set_7bit_address(i2c, address);
        i2c_set_read_transfer_dir(i2c);
        i2c_set_bytes_to_transfer(i2c, rxlen);
        /* start transfer */
        i2c_send_start(i2c);
        /* important to do it afterwards to do a proper repeated start! */
        i2c_enable_autoend(i2c);

        for (size_t i = 0; i < rxlen; i++) {
            while (i2c_received_data(i2c) == 0);
            rxbuf[i] = i2c_get_data(i2c);
        }
    }
    return true;
}

void I2Cd_Init(void)
{
    rcc_periph_clock_enable(RCC_I2C2);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_set_i2c_clock_hsi(I2C2);

    i2c_reset(I2C2);
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO14 | GPIO13);
    gpio_set_af(GPIOB, GPIO_AF5, GPIO13 | GPIO14);
    i2c_peripheral_disable(I2C2);

    i2c_enable_analog_filter(I2C2);
    i2c_set_digital_filter(I2C2, 0);

    i2c_set_speed(I2C2, i2c_speed_fm_400k, 8);
    i2c_enable_stretching(I2C2);

    i2c_set_7bit_addr_mode(I2C2);
    i2c_peripheral_enable(I2C2);
}

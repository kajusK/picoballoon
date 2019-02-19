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

/**
 * Driver layer for ublox gps module, without nmea decoder
 */

#include <string.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>

#include <gps.h>
#include "drivers/gps.h"

#define SYNC_CHAR1 0xb5
#define SYNC_CHAR2 0x62
#define UBX_CLASS_CFG 0x06

/** Power mode setup */
#define UBX_CMD_PMS 0x86

typedef enum {
    CFG_PMS_FULL,
    CFG_PMS_BALANCED,
    CFG_PMS_INTERVAL,
    CFG_PMS_AGGRESIVE_1HZ,
    CFG_PMS_AGGRESIVE_2HZ,
    CFG_PMS_AGGRESIVE_4HZ,
} ubx_cfg_pms_power_t;

typedef struct {
    uint8_t version;
    uint8_t power;
    uint16_t period;
    uint16_t onTime;
    uint8_t reserved;
} __attribute__((packed)) ubx_cfg_pms_t;

/** Power state */
#define UBX_CMD_PWR 0x57

typedef enum {
    CFG_PWR_GNSS_RUN = 0x52554E20,
    CFG_PWR_GNSS_STOP = 0x53544F50,
    CFG_PWR_SW_BACKUP = 0x42434B50, /* hw reboot required to start */
} ubx_cfg_pwr_state_t;

typedef struct {
    uint8_t version;
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
    uint32_t state;
} __attribute__((packed)) ubx_cfg_pwr_t;

void usart2_isr(void)
{
    uint8_t data;

    if ((USART_ISR(USART2) & USART_FLAG_RXNE) == 0) {
        return;
    }

    data = usart_recv(USART2);
    GPS_Process((char) data);
}

static void GPSd_Write(const uint8_t *data, uint16_t len)
{
    while (len-- != 0) {
        usart_send_blocking(USART2, *data++);
    }
}

static void GPSdi_UBXSend(uint8_t msg_id, uint8_t *payload, uint16_t len)
{
    uint8_t ck_a = 0;
    uint8_t ck_b = 0;

    uint8_t msg[50];
    msg[0] = SYNC_CHAR1;
    msg[1] = SYNC_CHAR2;
    msg[2] = UBX_CLASS_CFG;
    msg[3] = msg_id;
    msg[4] = len & 0xff;
    msg[5] = (len >> 8) & 0xff;

    memcpy(&msg[6], payload, len);

    for (int i = 0; i < len + 4; i++) {
        ck_a = ck_a + msg[i+2];
        ck_b = ck_b + ck_a;
    }

    msg[len+6] = ck_a;
    msg[len+7] = ck_b;

    GPSd_Write(msg, len + 8);
}

void GPSd_SetPowerSave(void)
{
    ubx_cfg_pms_t pms;

    pms.version = 0x00;
    pms.power = CFG_PMS_AGGRESIVE_1HZ;
    pms.period = 0;
    pms.onTime = 0;
    pms.reserved = 0;

    GPSdi_UBXSend(UBX_CMD_PMS, (uint8_t *) &pms, sizeof(pms));
}

void GPSd_SetPower(bool on)
{
    ubx_cfg_pwr_t pwr;

    pwr.version = 0x01;
    pwr.reserved1 = 0;
    pwr.reserved2 = 0;
    pwr.reserved3 = 0;
    if (on) {
        pwr.state = CFG_PWR_GNSS_RUN;
    } else {
        pwr.state = CFG_PWR_GNSS_STOP;
    }

    GPSdi_UBXSend(UBX_CMD_PWR, (uint8_t *) &pwr, sizeof(pwr));
}

void GPSd_Init(void)
{
    rcc_periph_clock_enable(RCC_USART2);
    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3 | GPIO2);
    gpio_set_af(GPIOA, GPIO_AF1, GPIO3 | GPIO2);

    usart_set_baudrate(USART2, 9600);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

    nvic_enable_irq(NVIC_USART2_IRQ);
    usart_enable_rx_interrupt(USART2);

    usart_enable(USART2);
}

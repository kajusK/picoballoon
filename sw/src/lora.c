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
#include <string.h>
#include <stdio.h>
#include <lmic.h>
#include <lmic/hal.h>

#include "config.h"
#include "lora.h"

static const uint8_t appeui[8] = APPEUI;
static const uint8_t devui[8] = DEVUI;
static const uint8_t appkey[16] = APPKEY;

/*
 * Functions required by lmic library
 */
void os_getArtEui(uint8_t *buf)
{
    memcpy(buf, appeui, 8);
}

void os_getDevEui(uint8_t *buf)
{
    memcpy(buf, devui, 8);
}

void os_getDevKey(uint8_t *buf)
{
    memcpy(buf, appkey, 16);
}

void onEvent (ev_t ev) {
    printf("\n\r");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            puts("EV_SCAN_TIMEOUT");
            break;
        case EV_BEACON_FOUND:
            puts("EV_BEACON_FOUND");
            break;
        case EV_BEACON_MISSED:
            puts("EV_BEACON_MISSED");
            break;
        case EV_BEACON_TRACKED:
            puts("EV_BEACON_TRACKED");
            break;
        case EV_JOINING:
            puts("EV_JOINING");
            break;
        case EV_JOINED:
            puts("EV_JOINED");
            /*
             * Disable link check validation (automatically enabled
             * during join, but not supported by TTN at this time).
             */
            LMIC_setLinkCheckMode(0);
            break;
        case EV_RFU1:
            puts("EV_RFU1");
            break;
        case EV_JOIN_FAILED:
            puts("EV_JOIN_FAILED");
            break;
        case EV_REJOIN_FAILED:
            puts("EV_REJOIN_FAILED");
            break;
        case EV_TXCOMPLETE:
            puts("EV_TXCOMPLETE + RX complete ");
            if (LMIC.txrxFlags & TXRX_ACK) {
                puts("Received ack ");
            }
            if (LMIC.dataLen) {
                printf("Received %d bytes\n", LMIC.dataLen);
                for (int i = 0; i < LMIC.dataLen; i++) {
                    printf("%x ", LMIC.frame[LMIC.dataBeg+1]);
                }
                putchar('\n');
            }
            // Schedule next transmission
            //os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            puts("EV_LOST_TSYNC");
            break;
        case EV_RESET:
            puts("EV_RESET");
            break;
        case EV_RXCOMPLETE:
            /* data received in ping slot */
            puts("EV_RXCOMPLETE");
            break;
        case EV_LINK_DEAD:
            puts("EV_LINK_DEAD");
            break;
        case EV_LINK_ALIVE:
            puts("EV_LINK_ALIVE");
            break;
         default:
            puts("Unknown event");
            break;
    }
}

bool Lora_Send(uint8_t *data, uint16_t len)
{
    /* Check if there is not a current TX/RX job running */
    if (LMIC.opmode & OP_TXRXPEND) {
        printf ("OP_TXRXPEND, not sending");
        return false;
    } else {
        /* Prepare upstream data transmission at the next possible time */
        LMIC_setTxData2(1, data, len, 0);
        printf("Packet queued");
        return true;
    }
    /* Next TX is scheduled after TX_COMPLETE event. */
}

bool Lora_IsAllSent(void)
{
    if (LMIC.opmode & OP_TXRXPEND) {
        return true;
    }
    return false;
}

void Lora_Update(void)
{
    os_runloop_once();
}

void Lora_PowerOff(void)
{
    LMIC_shutdown();
}

void Lora_PowerOn(void)
{
    LMIC_reset();
    LMIC_setClockError(MAX_CLOCK_ERROR * 50 / 100);
}

void Lora_Init(void)
{
    os_init();
    Lora_PowerOn();
}


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
            printf("EV_SCAN_TIMEOUT");
            break;
        case EV_BEACON_FOUND:
            printf("EV_BEACON_FOUND");
            break;
        case EV_BEACON_MISSED:
            printf("EV_BEACON_MISSED");
            break;
        case EV_BEACON_TRACKED:
            printf("EV_BEACON_TRACKED");
            break;
        case EV_JOINING:
            printf("EV_JOINING");
            break;
        case EV_JOINED:
            printf("EV_JOINED");
            /*
             * Disable link check validation (automatically enabled
             * during join, but not supported by TTN at this time).
             */
            LMIC_setLinkCheckMode(0);
            break;
        case EV_RFU1:
            printf("EV_RFU1");
            break;
        case EV_JOIN_FAILED:
            printf("EV_JOIN_FAILED");
            break;
        case EV_REJOIN_FAILED:
            printf("EV_REJOIN_FAILED");
            break;
        case EV_TXCOMPLETE:
            printf("EV_TXCOMPLETE + RX complete ");
            if (LMIC.txrxFlags & TXRX_ACK) {
                printf("Received ack ");
            }
            if (LMIC.dataLen) {
                printf("Received %d bytes", LMIC.dataLen);
            }
            // Schedule next transmission
            //os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            printf("EV_LOST_TSYNC");
            break;
        case EV_RESET:
            printf("EV_RESET");
            break;
        case EV_RXCOMPLETE:
            /* data received in ping slot */
            printf("EV_RXCOMPLETE");
            break;
        case EV_LINK_DEAD:
            printf("EV_LINK_DEAD");
            break;
        case EV_LINK_ALIVE:
            printf("EV_LINK_ALIVE");
            break;
         default:
            printf("Unknown event");
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

void Lora_Update(void)
{
    os_runloop_once();
}

void Lora_Init(void)
{
    os_init();
    LMIC_reset();
}


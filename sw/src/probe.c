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

#include <stdio.h>

#include "drivers/systick.h"
#include "drivers/adc.h"
#include "gps.h"
#include "sensors.h"
#include "lora.h"
#include "main.h"

#include "probe.h"

/** Allowed ranges for core voltage */
#define CORE_MV_MIN 3000
#define CORE_MV_MAX 3500

/** Time to wait for gps fix before skipping the gps */
#define MAX_GPS_FIX_WAIT_S 50

/** Data sending period (if cap has enough charge to keep mcu on) - every 30 min */
#define SENDING_PERIOD_S 1800

/** If sent more than given amout of messages, do the hw reset */
#define REBOOT_AFTER_CNT_MSG 10

/**
 * Critical voltage level that should cause mcu to kill all power and wait for
 * recharge
 */
#define MIN_CAP_MV 900

typedef struct {
    uint8_t vcc_ok : 1; /* MCU Vcc in expected ranges */
    uint8_t gps_fix : 1; /* We've got correct position */
    uint8_t charging : 1; /* Sun is shining, supercap is being charged */
    uint8_t msg_id : 5; /* Amount of messages sent since boot */
} state_bf_t;

typedef struct {
    uint8_t bf;         /* Bitfield with probe state */
    uint16_t cap_mv;    /* power supply voltage */
    uint16_t pressure_dmbar; /* Pressure in deci mBar - 11002 = 1100,2 mBar */
    int16_t temp_cc;    /* Temperature in centi deg C -5020 = -50,2 deg C */
    int8_t temp_c;      /* Temperature from another sensor in deg C */
    int8_t core_c;      /* Temperature of the system core */
    uint16_t altitude_m; /* altitude from gps */
    float lat;       /* [+-]dddmm.mmmmm */
    float lon;       /* [+-]dddmm.mmmmm */
}__attribute__((packed)) lora_msg_t;

/** amount of messages already sent */
static uint16_t probei_msg_sent = 0;


/**
 * Send data over lora
 */
static void Probe_Send(void)
{
    lora_msg_t msg;
    uint32_t pressure;
    int32_t temp;
    uint16_t cap_mv = Sensors_GetCapMv();
    uint16_t solar_mv = Sensors_GetSolarMv();
    uint16_t core_mv = Adcd_ReadVccMv();
    bool with_gps = Gps_GotFix();
    state_bf_t bf;

    printf("Sending packet %d\n", probei_msg_sent);

    Sensors_PressurecMbar(&pressure, &temp);

    bf.vcc_ok = core_mv > CORE_MV_MIN && core_mv < CORE_MV_MAX;
    bf.charging = solar_mv > cap_mv;
    bf.gps_fix = with_gps;
    bf.msg_id = probei_msg_sent >= 64 ? 63 : probei_msg_sent;

    msg.bf = *((uint8_t *) &bf);
    msg.cap_mv = cap_mv;
    msg.pressure_dmbar = pressure / 10;
    msg.temp_cc = temp;
    msg.temp_c = Sensors_TempDegC();
    msg.core_c = Adcd_ReadTempDegC();

    if (with_gps) {
        struct minmea_sentence_gga *frame;
        frame = Gps_GetGga();
        msg.altitude_m = frame->altitude.value / frame->altitude.scale;
        msg.lat = minmea_tocoord(&frame->latitude);
        msg.lon = minmea_tocoord(&frame->longitude);
    } else {
        msg.altitude_m = 0;
        msg.lat = 0;
        msg.lon = 0;
    }

    Lora_Send((uint8_t *) &msg, sizeof(msg));
    probei_msg_sent++;
    puts("Sent");
}

void Probe_Loop(void)
{
    static uint32_t last_sent_time = 0;
    bool timeouted = false;
    uint16_t cap_mv = Sensors_GetCapMv();

    /** Almost discharged, try to send data and die */
    if (cap_mv < MIN_CAP_MV) {
        puts("Supply almost dead, send and die.");
        Probe_Send();
        Sys_Shutdown();
    }

    //TODO sleep until gps fix received or timeouted or adc below limit

    if ((millis() - last_sent_time) > MAX_GPS_FIX_WAIT_S*1000) {
        printf("No GPS fix within specified period of time!");
        timeouted = true;
    }

    /* Have fix or timeouted, send data and go to sleep */
    if (Gps_GotFix() != false || timeouted) {
        Probe_Send();
        if (probei_msg_sent >= REBOOT_AFTER_CNT_MSG) {
            Sys_Shutdown();
        }
        Sys_Sleep(SENDING_PERIOD_S);
        last_sent_time = millis();
    }
}

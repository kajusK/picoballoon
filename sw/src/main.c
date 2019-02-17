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

#include <inttypes.h>
#include <stdio.h>

#include "drivers/wdg.h"
#include "drivers/systick.h"
#include "drivers/gps.h"
#include "drivers/adc.h"
#include "drivers/stdout.h"
#include "drivers/i2c.h"
#include "drivers/power.h"
#include "gps.h"
#include "sensors.h"
#include "lora.h"

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
    uint8_t msg_id : 6; /* Amount of messages sent since boot */
} state_bf_t;

typedef struct {
    state_bf_t bf;
    uint16_t cap_mv;    /* power supply voltage */
    uint16_t pressure_dmbar; /* Pressure in deci mBar - 11002 = 1100,2 mBar */
    int16_t temp_cc;    /* Temperature in centi deg C -5020 = -50,2 deg C */
    int8_t temp_c;      /* Temperature from another sensor in deg C */
    int8_t core_c;      /* Temperature of the system core */
    uint16_t altitude_m; /* altitude from gps */
}__attribute__((packed)) lora_msg_t;

/** amount of messages already sent */
static uint16_t probei_msg_sent = 0;

/**
 * Cut the power input
 */
static void Probe_ShutDown(void)
{
    puts("Shutting down... Don't want do die yet, I'm too young!\n");
    /* Wait for lora to send all remaining data */
    while (!Lora_IsAllSent()) {
        Lora_Update();
    }
    Powerd_ShutDown();
}

/**
 * Go to sleep mode for extended period of time, kill all modules around
 */
static void Probe_Sleep(void)
{
    puts("Going to sleep. Good night.\n");
    while (!Lora_IsAllSent()) {
        Lora_Update();
    }
    GPSd_SetPower(false);
    Lora_PowerOff();
    Wdgd_Clear();

    Powerd_Sleep(SENDING_PERIOD_S*1000U);

    Wdgd_Clear();
    Lora_PowerOn();
    GPSd_SetPower(true);
}

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
    bool with_gps = GPS_get_data() != NULL;

    printf("Sending packet %d\n", probei_msg_sent);

    Sensors_PressurecMbar(&pressure, &temp);

    msg.bf.vcc_ok = core_mv > CORE_MV_MIN && core_mv < CORE_MV_MAX;
    msg.bf.charging = solar_mv > cap_mv;
    msg.bf.gps_fix = with_gps;
    msg.bf.msg_id = probei_msg_sent >= 64 ? 63 : probei_msg_sent;

    msg.cap_mv = cap_mv;
    msg.pressure_dmbar = pressure / 10;
    msg.temp_cc = temp;
    msg.temp_c = Sensors_TempDegC();
    msg.core_c = Adcd_ReadTempDegC();

    //TODO gps

    Lora_Send((uint8_t *) &msg, sizeof(msg));
    probei_msg_sent++;
    puts("Sent\n");
}

/**
 * Main probe logic, try to send data if ready, else sleep or die
 */
static void Probe_Loop(void)
{
    static uint32_t last_sent_time = 0;
    bool timeouted = false;
    uint16_t cap_mv = Sensors_GetCapMv();

    /** Almost discharged, try to send data and die */
    if (cap_mv < MIN_CAP_MV) {
        puts("Supply almost dead, send and die\n");
        Probe_Send();
        Probe_ShutDown();
    }

    if ((millis() - last_sent_time) > MAX_GPS_FIX_WAIT_S*1000) {
        printf("No GPS fix within specified period of time\n");
        timeouted = true;
    }

    /* Have fix or timeouted, send data and go to sleep */
    if (GPS_get_data() != NULL || timeouted) {
        Probe_Send();
        if (probei_msg_sent >= REBOOT_AFTER_CNT_MSG) {
            Probe_ShutDown();
        }
        Probe_Sleep();
        last_sent_time = millis();
    }
}

static void Probe_Debug(void)
{
    uint16_t cap_mv = Sensors_GetCapMv();
    uint16_t solar_mv = Sensors_GetSolarMv();
    uint16_t core_mv = Adcd_ReadVccMv();
    int16_t core_c = Adcd_ReadTempDegC();
    int8_t temp2 = Sensors_TempDegC();
    uint32_t pressure;
    int32_t temp;

    Sensors_PressurecMbar(&pressure, &temp);

    puts("\n\n-----------\nProbe debug:\n");
    printf("Capacitor: %d mV\n", cap_mv);
    printf("Solar cell: %d mV\n", solar_mv);
    printf("Core voltage: %d mV\n", core_mv);
    printf("Core temperature: %d C\n", core_c);
    printf("Pressure: %ld mbar\n", pressure / 100);
    printf("Temperature: %ld C\n", temp / 100);
    printf("Temperature sensor: %d C\n", temp2);

    if (GPS_get_data() == NULL) {
        puts("GPS fig: No\n----------\n");
        return;
    }
    //TODO gps data
    //
    puts("-------------\n\n");
}

int main(void)
{
    Wdgd_Init();
    Stdoutd_Init();
    puts("Booting\n");
    puts("Brno Observatory and Planetarium Pico Balloon Challange 2019\n");
    puts("Team DeadBadger, Jakub Kaderka 2019\n");
    puts("-------------------------\n");

    Systickd_Init();
    Adcd_Init();

    GPSd_Init();
    GPSd_SetPowerSave();
    GPSd_SetPower(true);

    I2Cd_Init();
    Sensors_Init();
    Lora_Init();

    while (1) {
        Lora_Update();
        Probe_Loop();
        Wdgd_Clear();
    }
}

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
#include <string.h>

#include "drivers/systick.h"
#include "gps.h"

#define GPS_MSG_MAX_SIZE 100
#define MAX_FIX_AGE_MIN 30

static struct {
    char data[GPS_MSG_MAX_SIZE];
    uint8_t pos;
} gps_buf;

static uint32_t last_fix_ms = 0;
static struct minmea_sentence_gga gga_frame;
static uint32_t fix_time = 0;

static bool Gps_ProcessNmea(const char *msg)
{
    switch (minmea_sentence_id(msg, true)) {
         case MINMEA_SENTENCE_GGA:
              if (!minmea_parse_gga(&gga_frame, msg)) {
                  return false;
              }
              fix_time = millis();
              return true;
        default:
              return false;

    }

    return true;
}

bool Gps_ProcessChar(char c)
{
    /* first character of message */
    if (c == '$') {
        gps_buf.pos = 1;
        gps_buf.data[0] = c;
        return false;
    }

    if (gps_buf.pos == 0 || gps_buf.pos >= GPS_MSG_MAX_SIZE-1) {
        gps_buf.pos = 0;
        return false;
    }

    gps_buf.data[gps_buf.pos] = c;
    gps_buf.pos++;

    /* have complete message */
    if (c == '\n' || c == '\r') {
        gps_buf.data[gps_buf.pos] = '\0';
        return Gps_ProcessNmea(gps_buf.data);
    }

    return false;
}

bool Gps_GotFix(void)
{
    if (gga_frame.fix_quality > 0 && fix_time != 0) {
        return true;
    }
    return false;
}

void Gps_FixClear(void)
{
    fix_time = 0;
    memset(&gga_frame, 0, sizeof(gga_frame));
}

struct minmea_sentence_gga *Gps_GetGga(void)
{
    return &gga_frame;
}

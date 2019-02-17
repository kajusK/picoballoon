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
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "drivers/systick.h"
#include "gps.h"

#define GPS_MSG_MAX_SIZE 80

/* return 1 if comma moved to end of line, else continue */
#define COMMA_OR_RETURN(str) { p = comma(str); if (!p) return 1; }

#define parse_unit(c, def) (c == ',' ? def : toupper(c))
#define parse_num(c) ((c) == ',' ? 0 : (c) - '0')

struct s_gps_msg {
    char data[GPS_MSG_MAX_SIZE];
    uint8_t pos;
    /* packet started with $ */
    uint8_t got_start : 1;
    /* just processing the checksum frame */
    uint8_t in_checksum : 1;
    /* received checksum */
    uint8_t rec_checksum;
    /* calculated checksum */
    uint8_t checksum;
};

static uint16_t gps_frame_error = 0;
static uint16_t gps_checksum_error = 0;
static struct s_gps_msg msg;
static struct s_gps_RMC rmc;

static uint32_t last_fix_ms = 0;

inline static float knot_to_kmh(float speed)
{
    return speed*1.852;
}

inline static int char_to_hex(char c)
{
    if (isdigit(c))
        return c - '0';

    if (islower(c))
        return c - 'a';

    return c - 'A';
}

/* move pointer to first comma+1 in string or to the \0 at the end */
inline static char *comma(char *p)
{
    while (*p != '\0') {
        if (*p == ',')
            return p+1;
        p++;
    }

    return p;
}

static uint8_t parse_triplet(char *p, uint8_t *first, uint8_t *sec, uint8_t *third)
{
    if (strlen(p) < 6)
        return 1;

    *first = (p[0] - '0')*10 + (p[1] - '0');
    *sec = (p[2] - '0')*10 + (p[3] - '0');
    *third = (p[4] - '0')*10 + (p[5] - '0');
    return 0;
}

/* parse 01234.123 into 012 deg, 34 min and 123 hundreds */
static uint8_t parse_latlon(char *p, struct s_gps_latlon *latlon)
{
    char *pos = p;
    char *dot = p;
    int div = 10;

    while (*dot != '.' && *dot != ',' && *dot != '\0')
        dot++;

    if (*dot != '.')
        return 1;

    if (dot - pos == 4) {
        latlon->deg = (p[0] - '0')*10 + p[1] - '0';
        latlon->min = (p[2] - '0')*10 + p[3] - '0';
    } else if (dot - pos == 5) {
        latlon->deg = (p[0] - '0')*100 + (p[1] - '0')*10 + p[2] - '0';
        latlon->min = (p[3] - '0')*10 + p[4] - '0';
    } else {
        return 1;
    }

    pos = dot + 1;
    while (*pos != '\0' && *pos != ',') {
        latlon->min +=  ((float)((*pos) - '0'))/div;
        pos++;
        div *= 10;
    }

    return 0;
}

static float parse_float(char *p)
{
    char *dot = p;
    char *pos = p;
    int num = 0;
    float des = 0;

    while (*dot != '.' && *dot != ',' && *dot != '\0')
        dot++;

    while (pos != dot) {
        num = 10*num + (*pos)-'0';
        pos++;
    }

    if (*dot == ',' || *dot == '\0')
        return num;

    while (*pos != ',' && *pos != '\0')
        pos++;

    pos--;
    while (pos != dot) {
        des += (*pos)-'0';
        des /= 10;
        pos--;
    }

    return num + des;
}

static int parse_int(char *p)
{
    char *pos = p;
    int num = 0;

    if (p[0] == '-' || p[0] == '+')
        pos += 1;

    while (*pos != ',' && *pos  != '\0') {
        num = num*10 + (*pos) - '0';
        pos++;
    }

    if (p[0] == '-')
        return -num;
    return num;
}

/*
 * Recommended minimum
 *
 * GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W
 */
static uint8_t RMC_decode(char *str)
{
    char *p;
    uint8_t year;
    rmc.valid = 0;

    COMMA_OR_RETURN(str); //fix taken timestamp
    if (parse_triplet(p, &rmc.last_fix.hour, &rmc.last_fix.minute,
        &rmc.last_fix.second) != 0)
        return 1;

    COMMA_OR_RETURN(p); //status
    rmc.status = toupper(p[0]) == 'A' ? RMC_STATUS_ACTIVE : RMC_STATUS_VOID;

    COMMA_OR_RETURN(p); //latitude
    parse_latlon(p, &rmc.latitude);

    COMMA_OR_RETURN(p); //latitude direction
    rmc.latitude.dir = parse_unit(p[0], 'N');

    COMMA_OR_RETURN(p); //longitude
    parse_latlon(p, &rmc.longitude);

    COMMA_OR_RETURN(p); //longitude direction
    rmc.longitude.dir = parse_unit(p[0], 'E');

    COMMA_OR_RETURN(p); //speed over groun in knots
    rmc.speed_knots = parse_float(p);

    COMMA_OR_RETURN(p); //heading in degrees
    rmc.direction_deg = parse_float(p);

    COMMA_OR_RETURN(p); //date
    if (parse_triplet(p, &rmc.date.day, &rmc.date.month, &year) != 0)
        return 1;
    rmc.date.year = 2000 + year;

    COMMA_OR_RETURN(p); //magnetic variation
    rmc.mag_variation = parse_float(p);

    COMMA_OR_RETURN(p); //magnetic variation direction
    rmc.mag_variation_d = parse_unit(p[0], 'W');

    if (rmc.status != RMC_STATUS_ACTIVE)
        return 1;

    rmc.valid = 1;
    last_fix_ms = millis();
    return 0;
}

uint8_t NMEA_decode(char *str)
{
    if (strlen(str) < 6)
        return 1;

    if (strncmp(str, "GPRMC", 5) == 0)
        return RMC_decode(str);

    return 2;
}

uint8_t GPS_Process(char c)
{
    /* first character of message */
    if (c == '$') {
        msg.got_start = 1;
        msg.pos = 0;
        msg.checksum = 0;
        msg.in_checksum = 0;
        return 0;
    }
    if (!msg.got_start)
        return 0;

    /* have complete message */
    if (c == '\n' || c == '\r') {
        msg.got_start = 0;

        if (!msg.in_checksum) {
            gps_frame_error++;
            return 0;
        }
        if (msg.checksum != msg.rec_checksum) {
            gps_checksum_error++;
            return 0;
        }

        msg.data[msg.pos] = '\0';
        if (NMEA_decode(msg.data) == 0)
            return 1;
        return 0;
    }

    /* checksum field will follow */
    if (c == '*') {
        msg.in_checksum = 1;
        msg.rec_checksum = 0;
        return 0;
    }

    if (msg.in_checksum) {
        msg.rec_checksum = msg.rec_checksum*16 + char_to_hex(c);
        return 0;
    }

    if (msg.pos >= GPS_MSG_MAX_SIZE-1)
        return 0;

    msg.data[msg.pos] = c;
    msg.pos++;
    msg.checksum ^= c;
    return 0;
}

struct tm GPS_get_fix_time(void)
{
    struct tm time = {0};

    if (rmc.valid) {
        time.tm_hour = rmc.last_fix.hour;
        time.tm_min = rmc.last_fix.minute;
        time.tm_sec = rmc.last_fix.second;
        time.tm_year = rmc.date.year-1900;
        time.tm_mon = rmc.date.month - 1;
        time.tm_mday = rmc.date.day;
    }

    return time;
}

float GPS_get_speed_kmh(void)
{
    if (rmc.valid)
        return knot_to_kmh(rmc.speed_knots);

    return NAN;
}

uint32_t GPS_ms_since_fix(void)
{
    if (last_fix_ms == 0)
        return 0;
    return millis() - last_fix_ms + 1;
}

struct s_gps_data *GPS_get_data(void)
{
    static struct s_gps_data data;

    if (!rmc.valid)
        return NULL;

    data.ms_since_fix = GPS_ms_since_fix();
    data.time = rmc.last_fix;
    data.date = rmc.date;
    data.latitude = rmc.latitude;
    data.longitude = rmc.longitude;
    data.speed_kmh = GPS_get_speed_kmh();

    return &data;
}

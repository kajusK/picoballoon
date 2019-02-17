
#ifndef __GPS_H_
#define __GPS_H_

#include <inttypes.h>

enum e_rmc_status {
    RMC_STATUS_ACTIVE,
    RMC_STATUS_VOID
};

struct s_gps_time {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

struct s_gps_date {
    uint8_t day;
    uint8_t month;
    uint16_t year;
};

struct s_gps_latlon {
    uint8_t deg;
    float min;
    char dir;
};

struct s_gps_RMC {
    uint8_t valid;
    struct s_gps_time last_fix;
    struct s_gps_latlon latitude;
    struct s_gps_latlon longitude;
    enum e_rmc_status status;
    float speed_knots;
    float direction_deg;
    struct s_gps_date date;
    float mag_variation;
    char mag_variation_d;
};

struct s_gps_data {
    uint32_t ms_since_fix;
    struct s_gps_time time;
    struct s_gps_date date;
    struct s_gps_latlon latitude;
    struct s_gps_latlon longitude;
    float speed_kmh;
};

/*
 * Process character of incomming message and save it to the buffer
 * Return 1 if message was completed and processed
 */
extern uint8_t GPS_Process(char c);

/*
 * Parse nmea message into internal structures
 *
 * Return 0 if ok, 1 if failed, 2 if message type not known
 */
extern uint8_t NMEA_decode(char *str);

/*
 * Get time of latest fix
 */
extern struct tm GPS_get_fix_time(void);

//speed or NaN if not available
extern float GPS_get_speed_kmh(void);

//ms since last fix or 0 if fix not available
extern uint32_t GPS_ms_since_fix(void);

//null if not available
extern struct s_gps_data *GPS_get_data(void);

#endif

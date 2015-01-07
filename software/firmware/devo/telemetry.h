#ifndef _TELEMETRY_H_
#define _TELEMETRY_H_
#include <stdint.h>

#define NUM_DEVO_TELEM 9
#define NUM_DSM_TELEM  10
#define NUM_FRSKY_TELEM  7
#define NUM_TELEM (NUM_DEVO_TELEM > NUM_DSM_TELEM              \
                      ? (NUM_DEVO_TELEM > NUM_FRSKY_TELEM      \
                          ? NUM_DEVO_TELEM : NUM_FRSKY_TELEM)  \
                      : (NUM_DSM_TELEM > NUM_FRSKY_TELEM       \
                          ? NUM_DSM_TELEM : NUM_FRSKY_TELEM)   \
                   )
#define TELEM_ERROR_TIME 5000
#define TELEM_NUM_ALARMS 6

#define HAS_DSM_EXTENDED_TELEMETRY 0
enum {
    TELEM_DEVO,
    TELEM_DSM,
    TELEM_FRSKY,
};

enum {
    TELEM_DEVO_VOLT1 = 1,
    TELEM_DEVO_VOLT2,
    TELEM_DEVO_VOLT3,
    TELEM_DEVO_TEMP1,
    TELEM_DEVO_TEMP2,
    TELEM_DEVO_TEMP3,
    TELEM_DEVO_TEMP4,
    TELEM_DEVO_RPM1,
    TELEM_DEVO_RPM2,
    TELEM_DEVO_LAST,
};
enum {
    TELEM_DSM_FLOG_FADESA = 1,
    TELEM_DSM_FLOG_FADESB,
    TELEM_DSM_FLOG_FADESL,
    TELEM_DSM_FLOG_FADESR,
    TELEM_DSM_FLOG_FRAMELOSS,
    TELEM_DSM_FLOG_HOLDS,
    TELEM_DSM_FLOG_VOLT1,
    TELEM_DSM_FLOG_VOLT2,
    TELEM_DSM_FLOG_RPM1,
    TELEM_DSM_FLOG_TEMP1,
#if HAS_DSM_EXTENDED_TELEMETRY
    TELEM_DSM_AMPS1,
    TELEM_DSM_PBOX_VOLT1,
    TELEM_DSM_PBOX_VOLT2,
    TELEM_DSM_PBOX_CAPACITY1,
    TELEM_DSM_PBOX_CAPACITY2,
    TELEM_DSM_PBOX_ALARMV1,
    TELEM_DSM_PBOX_ALARMV2,
    TELEM_DSM_PBOX_ALARMC1,
    TELEM_DSM_PBOX_ALARMC2,
    TELEM_DSM_AIRSPEED,
    TELEM_DSM_ALTITUDE,
    TELEM_DSM_GFORCE_X,
    TELEM_DSM_GFORCE_Y,
    TELEM_DSM_GFORCE_Z,
    TELEM_DSM_GFORCE_XMAX,
    TELEM_DSM_GFORCE_YMAX,
    TELEM_DSM_GFORCE_ZMAX,
    TELEM_DSM_GFORCE_ZMIN,
    TELEM_DSM_JETCAT_STATUS,
    TELEM_DSM_JETCAT_THROTTLE,
    TELEM_DSM_JETCAT_PACKVOLT,
    TELEM_DSM_JETCAT_PUMPVOLT,
    TELEM_DSM_JETCAT_RPM,
    TELEM_DSM_JETCAT_TEMPEGT,
    TELEM_DSM_JETCAT_OFFCOND,
#endif //HAS_DSM_EXTENDED_TELEMETRY
    TELEM_DSM_LAST,
};

enum {
    TELEM_FRSKY_VOLT1 = 1,
    TELEM_FRSKY_VOLT2,
    TELEM_FRSKY_VOLT3,
    TELEM_FRSKY_TEMP1,
    TELEM_FRSKY_TEMP2,
    TELEM_FRSKY_RPM,
    TELEM_FRSKY_ALTITUDE,
    TELEM_FRSKY_LAST
};
#define TELEM_VALS        (((int)TELEM_DSM_LAST > (int)TELEM_DEVO_LAST)            \
                               ? (((int)TELEM_DSM_LAST > (int)TELEM_FRSKY_LAST)    \
                                   ? (int)TELEM_DSM_LAST : (int)TELEM_FRSKY_LAST)  \
                               : (((int)TELEM_DEVO_LAST > (int)TELEM_FRSKY_LAST)   \
                                   ? (int)TELEM_DEVO_LAST : (int)TELEM_FRSKY_LAST) \
                          )
enum {
    TELEM_GPS_LAT = TELEM_VALS,
    TELEM_GPS_LONG,
    TELEM_GPS_ALT,
    TELEM_GPS_SPEED,
    TELEM_GPS_TIME,
    TELEM_GPS_SATCOUNT,
    TELEM_GPS_HEADING,
};
enum {
    TELEMFLAG_ALARM1 = 0x00,
    TELEMFLAG_ALARM2 = 0x02,
    TELEMFLAG_ALARM3 = 0x04,
    TELEMFLAG_ALARM4 = 0x08,
    TELEMFLAG_ALARM5 = 0x10,
    TELEMFLAG_ALARM6 = 0x20,
};

enum {
    TELEMUNIT_FEET   = 0x40,
    TELEMUNIT_FAREN  = 0x80,
};
struct gps {
    int32_t latitude;
    int32_t longitude;
    int32_t altitude;
    int32_t velocity;
    uint32_t time;
    uint32_t heading;
    uint32_t satcount;
};

struct telem_devo {
    uint16_t volt[3];
    int16_t temp[4];
    uint16_t rpm[2];
};
struct telem_dsm_flog {
    //Do not change the order of these, they are aligned to the dsm packet
    uint8_t fades[4];
    uint8_t frameloss;
    uint8_t holds;
    uint16_t volt[2];
    uint16_t rpm;
    int16_t temp;
};

struct telem_dsm_pbox {
    uint16_t volt[2];
    uint16_t capacity[2];
    uint16_t alarmv[2];
    uint16_t alarmc[2];
};
struct telem_dsm_sensors {
    uint16_t amps;
    uint16_t airspeed;
    uint16_t altitude;
};
struct telem_dsm_gforce {
    uint16_t x;
    uint16_t y;
    uint16_t z;
    uint16_t xmax;
    uint16_t ymax;
    uint16_t zmax;
    uint16_t zmin;
};

struct telem_dsm_jetcat {
    uint8_t status;
    uint8_t offcond;
    uint16_t throttle;
    uint16_t packvolt;
    uint16_t pumpvolt;
    uint16_t rpm;
    uint16_t temp_egt;
};

struct telem_dsm {
    struct telem_dsm_flog    flog;
    struct telem_dsm_pbox    pbox;
    struct telem_dsm_sensors sensors;
    struct telem_dsm_gforce  gforce;
    struct telem_dsm_jetcat  jetcat;
};

struct telem_frsky {
    uint16_t volt[3];
    int16_t temp[2];
    uint16_t rpm;
    int32_t altitude;
    //uint16_t current;
    //uint16_t fuel;
};

#define TELEM_UPDATE_SIZE (((TELEM_VALS + 7)+ 7) / 8)
struct Telemetry {
    union {
        struct telem_devo  devo;
        struct telem_dsm   dsm;
        struct telem_frsky frsky;
    } p;
    struct gps gps;
    uint16_t capabilities;
    volatile uint8_t updated[TELEM_UPDATE_SIZE];
};

enum {
    PROTO_TELEM_UNSUPPORTED = -1,
    PROTO_TELEM_OFF = 0,
    PROTO_TELEM_ON  = 1,
};

extern struct Telemetry Telemetry; 
int32_t TELEMETRY_GetValue(int idx);
int32_t _TELEMETRY_GetValue(struct Telemetry *t, int idx);
const char * TELEMETRY_GetValueStr(char *str, unsigned telem);
const char * TELEMETRY_GetValueStrByValue(char *str, unsigned telem, int32_t value);
const char * TELEMETRY_Name(char *str, unsigned telem);
const char * TELEMETRY_ShortName(char *str, unsigned telem);
int32_t TELEMETRY_GetMaxValue(unsigned telem);
void TELEMETRY_Alarm(void);
int TELEMETRY_HasAlarm(int src);
uint32_t TELEMETRY_IsUpdated(int val);
void TELEMETRY_SetUpdated(int telem);
int TELEMETRY_Type(void);
void TELEMETRY_SetType(int type);
void TELEMETRY_SetTypeByProtocol(enum Protocols protocol);
int TELEMETRY_GetNumTelemSrc(void);
int TELEMETRY_GetNumTelem(void);
#endif

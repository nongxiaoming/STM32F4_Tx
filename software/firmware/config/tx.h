#ifndef _TX_H_
#define _TX_H_
#include "stm32f4xx.h"
#include "mixer.h"
#include "autodimmer.h"
#include "telemetry.h"

#define DEFAULT_BATTERY_WARNING_INTERVAL 30
#define MIN_BATTERY_WARNING_INTERVAL 0
#define MAX_BATTERY_WARNING_INTERVAL 60
#define DEFAULT_SPLASH_DELAY 35 //3.5sec

struct StickCalibration {
    uint16_t max;
    uint16_t min;
    uint16_t zero;
};

struct TouchCalibration {
    int32_t xscale;
    int32_t yscale;
    int32_t xoffset;
    int32_t yoffset;
};

struct mcu_pin {
    GPIO_TypeDef *port;
    uint16_t pin;
};

enum ExtraHardware {
    VIBRATING_MOTOR = 0x01,
    FORCE_MODULES   = 0x02,
};

// bitmap for rtcflags:
#define CLOCK12HR 0x01  //0b00000001
#define TIMEFMT   0x0F  //0b00001111
#define DATEFMT   0xF0  //0b11110000

struct Transmitter {
    uint8_t current_model;
    uint8_t language;
    uint8_t brightness;
    uint8_t contrast;
    uint8_t telem;
    uint8_t music_shutdown;
    uint8_t extra_hardware;
    enum Mode mode;
    uint16_t batt_alarm;
    uint8_t power_alarm;
    uint16_t batt_critical;
    uint16_t batt_warning_interval;
    uint8_t splash_delay;
    uint8_t volume;
    uint8_t vibration_state; // for future vibration on/off support
#if HAS_RTC
    uint8_t rtcflags;    // bit0: clock12hr, bit1-3: time format, bit4-7 date format (see pages/320x240x16/rtc_config.c)
#endif
    #ifdef HAS_MORE_THAN_32_INPUTS
        u64 ignore_src;
    #else
        uint32_t ignore_src;
    #endif
    struct mcu_pin module_enable[TX_MODULE_LAST];
    uint8_t module_poweramp;
    uint32_t txid;
    struct StickCalibration calibration[INP_HAS_CALIBRATION];
    struct TouchCalibration touch;
    struct AutoDimmer auto_dimmer;
    struct CountDownTimerSettings countdown_timer_settings;
};

extern struct Transmitter Transmitter;
#define MODULE_ENABLE Transmitter.module_enable

void CONFIG_LoadTx(void);
void CONFIG_LoadHardware(void);

#endif

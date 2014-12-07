#ifndef _MIXER_STANDARD_H_
#define _MIXER_STANDARD_H_

typedef enum {
    GYROOUTPUT_GEAR = 4, // CH5
    GYROOUTPUT_AUX2 = 6, // CH7
} GyroOutputChannel;

typedef enum {
    SWITCHFUNC_FLYMODE = 0,
    SWITCHFUNC_HOLD,
    SWITCHFUNC_GYROSENSE,
    SWITCHFUNC_DREXP_AIL, // ALLOW dr/exp to use different switch than fly mode,however, they use the same switch by default ,
    SWITCHFUNC_DREXP_ELE, // ALLOW dr/exp to use different switch than fly mode,however, they use the same switch by default ,
    SWITCHFUNC_DREXP_RUD, // ALLOW dr/exp to use different switch than fly mode,however, they use the same switch by default ,
    SWITCHFUNC_LAST
} FunctionSwitch;

typedef struct {
    uint8_t aile;
    uint8_t elev;
    uint8_t throttle;
    uint8_t rudd;
    uint8_t pitch;
    uint8_t gear;
    uint8_t aux2;
    uint8_t actual_aile;
    uint8_t actual_elev;
    uint8_t switches[SWITCHFUNC_LAST];
} MappedSimpleChannels;

#define MAX_TRAVEL_LIMIT 175
#define PITCHMIXER_COUNT 4
#define THROTTLEMIXER_COUNT 3
#define DREXPMIXER_COUNT 3
#define GYROMIXER_COUNT 3

extern MappedSimpleChannels mapped_std_channels;

#endif

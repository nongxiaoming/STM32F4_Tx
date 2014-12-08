#ifndef _MIXER_H_
#define _MIXER_H_
#include <stdint.h>

#define DEFAULT_SERVO_LIMIT 150
#define SWASH_INV_ELEVATOR_MASK   1
#define SWASH_INV_AILERON_MASK    2
#define SWASH_INV_COLLECTIVE_MASK 4

//MAX = 10000
//MIN = -10000
#define CHAN_MULTIPLIER 100
#define PCT_TO_RANGE(x) ((s16)(x) * CHAN_MULTIPLIER)
#define RANGE_TO_PCT(x) ((s16)(x) / CHAN_MULTIPLIER)
#define CHAN_MAX_VALUE (100 * CHAN_MULTIPLIER)
#define CHAN_MIN_VALUE (-100 * CHAN_MULTIPLIER)
#define NUM_CHANNELS (NUM_OUT_CHANNELS + NUM_VIRT_CHANNELS)
#define NUM_SOURCES (NUM_INPUTS + NUM_CHANNELS + MAX_PPM_IN_CHANNELS)

#define CURVE_TYPE(x)       (((x)->type) & 0x7F)
#define CURVE_SMOOTHING(x)  (((x)->type) & 0x80)
#define CURVE_SET_TYPE(x, y) ((x)->type = ((x)->type & ~0x7F) | (y))
#define CURVE_SET_SMOOTHING(x, y) ((x)->type = ((x)->type & ~0x80) | ((y) ? 0x80 : 0))

#define MIXER_MUX(x)        (((x)->flags) & 0x0F)
#define MIXER_APPLY_TRIM(x) (((x)->flags) & 0x10)
#define MIXER_SET_APPLY_TRIM(x,y) ((x)->flags = ((x)->flags & ~0x10) | ((y) ? 0x10 : 0))
#define MIXER_SET_MUX(x,y)        ((x)->flags = ((x)->flags & ~0x0F) | (y))

enum {
    TRIM_ONOFF     = 191,
    TRIM_TOGGLE    = 192,
    TRIM_MOMENTARY = 193,
    TRIM_SWITCH_TYPES = 3,
};

enum Safety {
    SAFE_NONE,
    SAFE_MIN,
    SAFE_ZERO,
    SAFE_MAX,
    SAFE_COUNT,
};

enum Mode {
    MODE_1=1,
    MODE_2=2,
    MODE_3=3,
    MODE_4=4
};

enum PPMInMode {
    PPM_IN_OFF,
    PPM_IN_TRAIN1,
    PPM_IN_TRAIN2,
    PPM_IN_SOURCE,
};

enum CurveType {
    CURVE_NONE,
    CURVE_FIXED,
    CURVE_MIN_MAX,
    CURVE_ZERO_MAX,
    CURVE_GT_ZERO,
    CURVE_LT_ZERO,
    CURVE_ABSVAL,
    CURVE_EXPO,
    CURVE_DEADBAND,
    CURVE_3POINT,
    CURVE_5POINT,
    CURVE_7POINT,
    CURVE_9POINT,
    CURVE_11POINT,
    CURVE_13POINT,
};
#define CURVE_MAX CURVE_13POINT


struct Curve {
    enum CurveType type;
    int8_t points[MAX_POINTS];
    //uint8_t index;
    //s8 p1;
    //s8 p2;
};

//The followingis defined bythe target
extern const char *tx_stick_names[4];

enum TemplateType {
    MIXERTEMPLATE_NONE,
    MIXERTEMPLATE_SIMPLE,
    MIXERTEMPLATE_EXPO_DR,
    MIXERTEMPLATE_COMPLEX,
    MIXERTEMPLATE_CYC1,
    MIXERTEMPLATE_CYC2,
    MIXERTEMPLATE_CYC3,
};
#define MIXERTEMPLATE_MAX_HELI MIXERTEMPLATE_CYC3
#define MIXERTEMPLATE_MAX_PLANE MIXERTEMPLATE_COMPLEX
#define MIXERTEMPLATE_MAX (MIXERTEMPLATE_MAX_HELI > MIXERTEMPLATE_MAX_PLANE ? MIXERTEMPLATE_MAX_HELI : MIXERTEMPLATE_MAX_PLANE)

enum MuxType {
    MUX_REPLACE,
    MUX_MULTIPLY,
    MUX_ADD,
    MUX_MAX,
    MUX_MIN,
    MUX_DELAY,
    MUX_LAST,
};

enum SwashType {
    SWASH_TYPE_NONE,
    SWASH_TYPE_120,
    SWASH_TYPE_120X,
    SWASH_TYPE_140,
    SWASH_TYPE_90,
    SWASH_TYPE_LAST,
};
#define MIXER_SRC(x) ((x) & 0x7F)
#define MIXER_SRC_IS_INV(x) ((x) & 0x80)
#define MIXER_SET_SRC_INV(x, y) x = (y) ? ((x) | 0x80) : ((x) & ~0x80)
struct Mixer {
    struct Curve curve;
    uint8_t src;
    uint8_t dest;
    uint8_t sw;
    int8_t scalar;
    int8_t offset;
    uint8_t flags;
    //apply_trim;
    //enum MuxType mux;
};

enum LimitFlags {
    CH_REVERSE     = 0x01,
    CH_FAILSAFE_EN = 0x02,
};

enum LimitMask {
    APPLY_REVERSE = 0x01,
    APPLY_LIMITS  = 0x02,
    APPLY_SAFETY  = 0x04,
    APPLY_SUBTRIM = 0x08,
    APPLY_SCALAR  = 0x10,
    APPLY_SPEED   = 0x80,
    APPLY_ALL     = 0xFF,
};

struct Limit {
    uint8_t flags;
    uint8_t safetysw;
    int16_t safetyval;  // allow safetyval to be over +/-125
    uint8_t max;
    uint8_t min;
    uint8_t servoscale;
    uint8_t servoscale_neg;
    int8_t failsafe;
    uint8_t speed;     //measured in degrees/100msec
    int16_t subtrim;  // need to support value greater than 250
};

struct Trim {
    uint8_t src;
    uint8_t pos;
    uint8_t neg;
    uint8_t step;
    uint8_t sw;
    int8_t value[3];
};

/* Curve functions */
int16_t CURVE_Evaluate(int16_t value, struct Curve *curve);
const char *CURVE_GetName(char *str, struct Curve *curve);
unsigned CURVE_NumPoints(struct Curve *curve);

/* Mixer functions */
volatile int16_t *MIXER_GetInputs(void);
int16_t MIXER_GetChannel(unsigned channel, enum LimitMask flags);

int MIXER_GetMixers(int ch, struct Mixer *mixers, int count);
int MIXER_SetMixers(struct Mixer *mixers, int count);

void MIXER_GetLimit(int ch, struct Limit *limit);
void MIXER_SetLimit(int ch, struct Limit *limit);

void MIXER_SetTemplate(int ch, enum TemplateType value);
enum TemplateType MIXER_GetTemplate(int ch);

void MIXER_InitMixer(struct Mixer *mixer, unsigned ch);

void MIXER_ApplyMixer(struct Mixer *mixer, volatile int16_t *raw, int16_t *orig_value);
void MIXER_EvalMixers(volatile int16_t *raw);
int MIXER_GetCachedInputs(int16_t *raw, unsigned threshold);

struct Mixer *MIXER_GetAllMixers(void);

struct Trim *MIXER_GetAllTrims(void);

void MIXER_RegisterTrimButtons(void);

int16_t MIXER_ApplyLimits(unsigned channel, struct Limit *limit, volatile int16_t *_raw,
                      volatile int16_t *_Channels, enum LimitMask flags);
void MIXER_SetDefaultLimit(struct Limit *limit);
const char *MIXER_TemplateName(enum TemplateType t);
const char *MIXER_SwashType(enum SwashType);
unsigned MIXER_SourceHasTrim(unsigned src);
unsigned MIXER_MapChannel(unsigned channel);
unsigned MIXER_UpdateTrim(uint32_t buttons, unsigned flags, void *data);
int8_t *MIXER_GetTrim(unsigned i);
int32_t MIXER_GetTrimValue(int i);

void MIXER_Init(void);

#endif

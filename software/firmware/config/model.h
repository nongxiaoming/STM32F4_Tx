#define NUM_TRIM_ELEMS 6
#define NUM_BOX_ELEMS 8
#define NUM_BAR_ELEMS 8
#define NUM_TOGGLE_ELEMS 4

#ifndef _MODEL_H_
#define _MODEL_H_

#include "mixer.h"
#include "timer.h"
#include "rtc.h"
#include "telemetry.h"
#include "datalog.h"
#include "pagecfg.h"

/* INI file consts */
const char *MODEL_NAME;
const char *MODEL_ICON;
const char *MODEL_TYPE;
const char *MODEL_TEMPLATE;
#define UNKNOWN_ICON "media/noicon.bmp"

//This cannot be computed, and must be manually updated
#define NUM_PROTO_OPTS 4
#define VIRT_NAME_LEN 10 

struct Model {
    char name[24];
    char icon[24];
    enum ModelType type;
    enum Protocols protocol;
    s16 proto_opts[NUM_PROTO_OPTS];
    uint8_t num_channels;
    uint8_t num_ppmin;
    uint16_t ppmin_centerpw;
    uint16_t ppmin_deltapw;
    uint8_t train_sw;
    s8 ppm_map[MAX_PPM_IN_CHANNELS];
    uint32_t fixed_id;
    enum TxPower tx_power;
    enum SwashType swash_type;
    uint8_t swash_invert;
    uint8_t swashmix[3];
    struct Trim trims[NUM_TRIMS];
    struct Mixer mixers[NUM_MIXERS];
    struct Limit limits[NUM_OUT_CHANNELS];
    char virtname[NUM_VIRT_CHANNELS][VIRT_NAME_LEN];
    struct Timer timer[NUM_TIMERS];
    uint8_t templates[NUM_CHANNELS];
    struct PageCfg2 pagecfg2;
    uint8_t safety[NUM_SOURCES+1];
    uint8_t telem_alarm[TELEM_NUM_ALARMS];
    uint16_t telem_alarm_val[TELEM_NUM_ALARMS];
    uint8_t telem_flags;
    MixerMode mixer_mode;
    uint32_t permanent_timer;
#if HAS_DATALOG
    struct datalog datalog;
#endif
};
extern struct Model Model;
extern const char * const RADIO_TX_POWER_VAL[TXPOWER_LAST];

uint8_t CONFIG_ReadModel(uint8_t model_num);
uint8_t CONFIG_WriteModel(uint8_t model_num);
uint8_t CONFIG_GetCurrentModel(void);
const char *CONFIG_GetIcon(enum ModelType type);
const char *CONFIG_GetCurrentIcon(void);
enum ModelType CONFIG_ParseModelType(const char *value);
void CONFIG_ParseIconName(char *name, const char *value);
void CONFIG_ResetModel(void);
uint8_t CONFIG_ReadTemplateByIndex(uint8_t template_num);
uint8_t CONFIG_ReadTemplate(const char *filename);
uint8_t CONFIG_ReadLayout(const char *filename);
#endif /*_MODEL_H_*/

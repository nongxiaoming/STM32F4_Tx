#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Magic macro to check enum size
//#define ctassert(n,e) extern unsigned char n[(e)?0:-1]
#define ctassert(COND,MSG) typedef char static_assertion_##MSG[(COND)?1:-1]


#define TEMPSTRINGLENGTH 400 //This is the max dialog size (80 characters * 5 lines)
                             //We could reduce this to ~240 on the 128x64 screens
                             //But only after all sprintf are replaced with snprintf
                             //Maybe move this to target_defs.h
extern char tempstring[TEMPSTRINGLENGTH];


typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef uint8_t uint8_t;
typedef uint16_t uint16_t;
typedef uint32_t uint32_t;
typedef uint64_t u64;

#include "target.h"
#include "std.h"

//FATFS is defined by target_defs.h
struct FAT {
    char a[FILE_SIZE];
};


extern volatile s16 Channels[NUM_OUT_CHANNELS];
extern const char DeviationVersion[33];

/* Temproary definition until we have real translation */
#define _tr_noop(x) x
#ifdef NO_LANGUAGE_SUPPORT
#define _tr(x) x
#else
const char *_tr(const char *str);
#endif

void CONFIG_ReadLang(uint8_t idx);
void CONFIG_EnableLanguage(int state);
int CONFIG_IniParse(const char* filename,
         int (*handler)(void*, const char*, const char*, const char*),
         void* user);
uint8_t CONFIG_IsModelChanged(void);
uint8_t CONFIG_SaveModelIfNeeded(void);
void CONFIG_SaveTxIfNeeded(void);
extern const char * const MODULE_NAME[TX_MODULE_LAST];

/* LCD primitive functions */
void LCD_Clear(unsigned int color);
    /* Strings */
void LCD_PrintCharXY(unsigned int x, unsigned int y, uint32_t c);
void LCD_PrintChar(uint32_t c);
void LCD_PrintStringXY(unsigned int x, unsigned int y, const char *str);
void LCD_PrintString(const char *str);
void LCD_SetXY(unsigned int x, unsigned int y);
void LCD_GetStringDimensions(const uint8_t *str, uint16_t *width, uint16_t *height);
void LCD_GetCharDimensions(uint32_t c, uint16_t *width, uint16_t *height);
uint8_t LCD_SetFont(unsigned int idx);
uint8_t  LCD_GetFont(void);
void LCD_SetFontColor(uint16_t color);
    /* Graphics */
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void LCD_FillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void LCD_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void LCD_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void LCD_DrawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);
void LCD_DrawDashedVLine(int16_t x, int16_t y, int16_t h, int16_t space, uint16_t color);
void LCD_DrawDashedHLine(int16_t x, int16_t y, int16_t w, int16_t space, uint16_t color);
void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void LCD_DrawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
void LCD_FillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
void LCD_DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_FillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawWindowedImageFromFile(uint16_t x, uint16_t y, const char *file, s16 w, s16 h, uint16_t x_off, uint16_t y_off);
void LCD_DrawImageFromFile(uint16_t x, uint16_t y, const char *file);
uint8_t LCD_ImageIsTransparent(const char *file);
uint8_t LCD_ImageDimensions(const char *file, uint16_t *w, uint16_t *h);
void LCD_DrawUSBLogo(int lcd_width, int lcd_height);

/* Music */

/* Mixer functions */
void MIXER_CalcChannels(void);

/* GUI Pages */
void PAGE_Init(void);
void PAGE_Change(int dir);
void PAGE_Event(void);
int PAGE_DialogVisible(void);
void PAGE_ShowSafetyDialog(void);
void PAGE_CloseBindingDialog(void);
void PAGE_ShowBindingDialog(uint8_t update);
void PAGE_ShowLowBattDialog(void);
void PAGE_DisableSafetyDialog(uint8_t disable);
void PAGE_ShowResetPermTimerDialog(void *guiObject, void *data);
void PAGE_ShowInvalidModule(void);
void PAGE_ShowModuleDialog(const char **missing);
void PAGE_ShowWarning(const char *title, const char *str);
const char *PAGE_GetName(int idx);
int PAGE_GetNumPages(void);
int PAGE_GetStartPage(void);

/* Protocol */
#define PROTODEF(proto, module, map, init, name) proto,
enum Protocols {
    PROTOCOL_NONE,
    #include "protocol.h"
    PROTOCOL_COUNT,
};
#undef PROTODEF
extern const uint8_t *ProtocolChannelMap[PROTOCOL_COUNT];
extern const char * const ProtocolNames[PROTOCOL_COUNT];
#define PROTO_MAP_LEN 5

enum ModelType {
    MODELTYPE_HELI,
    MODELTYPE_PLANE,
    MODELTYPE_LAST,
};

enum TxPower {
    TXPOWER_100uW,
    TXPOWER_300uW,
    TXPOWER_1mW,
    TXPOWER_3mW,
    TXPOWER_10mW,
    TXPOWER_30mW,
    TXPOWER_100mW,
    TXPOWER_150mW,
    TXPOWER_LAST,
};

void PROTOCOL_Init(uint8_t force);
void PROTOCOL_Load(int no_dlg);
void PROTOCOL_DeInit(void);
uint8_t PROTOCOL_WaitingForSafe(void);
u64 PROTOCOL_CheckSafe(void);
uint32_t PROTOCOL_Binding(void);
uint8_t PROTOCOL_AutoBindEnabled(void);
void PROTOCOL_Bind(void);
void PROTOCOL_SetBindState(uint32_t msec);
int PROTOCOL_NumChannels(void);
uint8_t PROTOCOL_GetTelemCapability(void);
int PROTOCOL_DefaultNumChannels(void);
void PROTOCOL_CheckDialogs(void);
uint32_t PROTOCOL_CurrentID(void);
const char **PROTOCOL_GetOptions(void);
void PROTOCOL_SetOptions(void);
int PROTOCOL_GetTelemetryState(void);
int PROTOCOL_MapChannel(int input, int default_ch);
int PROTOCOL_HasModule(int idx);
int PROTOCOL_HasPowerAmp(int idx);
int PROTOCOL_SetSwitch(int module);
int PROTOCOL_SticksMoved(int init);
void PROTOCOL_InitModules(void);


/* Input */
const char *INPUT_SourceName(char *str, unsigned src);
const char *INPUT_SourceNameReal(char *str, unsigned src);
const char *INPUT_SourceNameAbbrevSwitch(char *str, unsigned src);
const char *INPUT_SourceNameAbbrevSwitchReal(char *str, unsigned src);
int INPUT_GetAbbrevSource(int origval, int newval, int dir);
int INPUT_SwitchPos(unsigned src);
int INPUT_NumSwitchPos(unsigned src);
int INPUT_GetFirstSwitch(int src);
int INPUT_SelectSource(int src, int dir, uint8_t *changed);
int INPUT_SelectAbbrevSource(int src, int dir);

const char *INPUT_MapSourceName(unsigned idx, unsigned *val);
const char *INPUT_ButtonName(unsigned src);

/* Misc */
void Delay(uint32_t count);
uint32_t Crc(const void *buffer, uint32_t size);
const char *utf8_to_uint32_t(const char *str, uint32_t *ch);
int exact_atoi(const char *str); //Like atoi but will not decode a number followed by non-number
size_t strlcpy(char* dst, const char* src, size_t bufsize);
void tempstring_cpy(const char* src);
int fexists(const char *file);
uint32_t rand32_r(uint32_t *seed, uint8_t update); //LFSR based PRNG
uint32_t rand32(void); //LFSR based PRNG
extern volatile uint8_t priority_ready;
void medium_priority_cb(void);
void debug_timing(uint32_t type, int startend); //This is only defined if TIMING_DEBUG is defined
/* Battery */
#define BATTERY_CRITICAL 0x01
#define BATTERY_LOW      0x02
uint8_t BATTERY_Check(void);

/* Mixer mode */
typedef enum {
    MIXER_ADVANCED = 0,
    MIXER_STANDARD,
    MIXER_ALL,
} MixerMode;
void PAGE_ShowInvalidStandardMixerDialog(void *guiObj);
void STDMIXER_Preset(void);
void STDMIXER_SetChannelOrderByProtocol(void);
unsigned STDMIXER_ValidateTraditionModel(void);
const char *STDMIXER_ModeName(int mode);
void STDMIXER_InitSwitches(void);
void STDMIXER_SaveSwitches(void);
const char *GetElemName(int type);
const char *GetBoxSource(char *str, int src);
const char *GetBoxSourceReal(char *str, int src);
void RemapChannelsForProtocol(const uint8_t *oldmap);
#define PPMin_Mode(void) (Model.num_ppmin >> 6)
#endif

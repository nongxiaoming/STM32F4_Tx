#ifndef _TARGET_H_
#define _TARGET_H_
#include <stdint.h>

#define OPTIONAL 2
//Load target-specific include
#include "target_defs.h"

/* List of channels provided by the Tx */
#define CHANDEF(x) INP_##x,
#define UNDEF_INP
enum {
    INP_NONE,
    #include "capabilities.h"
    INP_LAST,
};
#undef CHANDEF
#undef UNDEF_INP
/* List of buttons provided by the Tx */
#define BUTTONDEF(x) BUT_##x,
enum {
    BUT_NONE,
    #include "capabilities.h"
    BUT_LAST,
};
#undef BUTTONDEF

#define CHAN_ButtonMask(btn) (btn ? (1 << (btn - 1)) : 0)

#define NUM_TX_INPUTS (INP_LAST - 1)
#define NUM_INPUTS (NUM_TX_INPUTS)
#define NUM_TX_BUTTONS (BUT_LAST - 1)

enum {
    CYRF6936=0x00,
    A7105=0x01,
    CC2500=0x02,
    NRF24L01=0x03,
    MULTIMOD=0x04,
    TX_MODULE_LAST=0x05,
};


#define SWITCH_ADDRESS 0xFFFFFFFF
/* The following functions must be provided by every target */

/* General Functions */
void TxName(uint8_t *var, int len);

/* Backlight Functions */
void BACKLIGHT_Init(void);
void BACKLIGHT_Brightness(unsigned brightness);

/* Display Functions */
void LCD_Init(void);
void LCD_Contrast(unsigned contrast);

    /* Primitives */
enum DrawDir {
    DRAW_NWSE,
    DRAW_SWNE,
};
void LCD_DrawPixel(unsigned int color);
void LCD_DrawPixelXY(unsigned int x, unsigned int y, unsigned int color);
void LCD_DrawStart(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, enum DrawDir dir);
void LCD_DrawStop(void);

/* Touchscreen */
struct touch {
    uint16_t x;
    uint16_t y;
    uint16_t z1;
    uint16_t z2;
};
void SPITouch_Init(void);
struct touch SPITouch_GetCoords(void);
int SPITouch_IRQ(void);
void SPITouch_Calibrate(int32_t xscale, int32_t yscale, int32_t xoff, int32_t yoff);

/* Buttons and switches */
void Initialize_ButtonMatrix(void);
uint32_t ScanButtons(void);

/* Power functions */
void PWR_Init(void);
unsigned  PWR_ReadVoltage(void);
int  PWR_CheckPowerSwitch(void);
void PWR_Shutdown(void);
void PWR_Sleep(void);

/* Clock functions */
#define LOW_PRIORITY_MSEC 100
#define MEDIUM_PRIORITY_MSEC   5
enum MsecCallback {
    MEDIUM_PRIORITY,
    LOW_PRIORITY,
    LAST_PRIORITY,
};

void CLOCK_Init(void);
uint32_t CLOCK_getms(void);
void CLOCK_StartTimer(unsigned us, uint16_t (*cb)(void));
void CLOCK_StopTimer(void);
void CLOCK_SetMsecCallback(int cb, uint32_t msec);
void CLOCK_StartWatchdog(void);
void CLOCK_ResetWatchdog(void);

/*PWM/PPM functions */
void PWM_Initialize(void);
void PWM_Stop(void);
void PWM_Set(int);
void PPM_Enable(unsigned low_time, volatile uint16_t *pulses);

/* PPM-In functions */
#define MAX_PPM_IN_CHANNELS 8
void PPMin_TIM_Init(void);
void PPMin_Start(void);
void PPMin_Stop(void);


/* Sticks */
void CHAN_Init(void);
int16_t  CHAN_ReadInput(int channel);
int32_t  CHAN_ReadRawInput(int channel);
extern void CHAN_SetSwitchCfg(const char *str);
#define CHAN_ButtonIsPressed(buttons, btn) (buttons & (CHAN_ButtonMask(btn)))

/* SPI Flash */
void SPIFlash_Init(void);
uint32_t  SPIFlash_ReadID(void);
void SPIFlash_EraseSector(uint32_t sectorAddress);
void SPIFlash_BulkErase(void);
void SPIFlash_WriteBytes(uint32_t writeAddress, uint32_t length, const uint8_t * buffer);
void SPIFlash_WriteByte(uint32_t writeAddress, const unsigned byte);
void SPIFlash_ReadBytes(uint32_t readAddress, uint32_t length, uint8_t * buffer);
int  SPIFlash_ReadBytesStopCR(uint32_t readAddress, uint32_t length, uint8_t * buffer);
void SPI_FlashBlockWriteEnable(unsigned enable);

/* Sound */
void SOUND_Init(void);
void SOUND_SetFrequency(unsigned freq, unsigned volume);
void SOUND_Start(unsigned msec, uint16_t (*next_note_cb)(void));
void SOUND_StartWithoutVibrating(unsigned msec, uint16_t(*next_note_cb)(void));
void SOUND_Stop(void);

/* Vibrating motor */
void VIBRATINGMOTOR_Init(void);
void VIBRATINGMOTOR_Start(void);
void VIBRATINGMOTOR_Stop(void);

/* UART & Debug */
void UART_Initialize(void);
void UART_Stop(void);

/* USB*/
void USB_Enable(unsigned type, unsigned use_interrupt);
void USB_Disable(void);
void USB_HandleISR(void);
void USB_Connect(void);
void HID_Enable(void);
void HID_Disable(void);

/* Filesystem */
int FS_Mount(void *FAT, const char *drive);
void FS_Unmount(void);
int FS_OpenDir(const char *path);
int FS_ReadDir(char *path);
void FS_CloseDir(void);

void _usleep(uint32_t usec);
#define usleep _usleep

/* Abstract bootloader access */
enum {
    BL_ID = 0,
};
uint8_t *BOOTLOADER_Read(int idx);

#define PROTO_HAS_CYRF6936
#define PROTO_HAS_A7105
#define PROTO_HAS_CC2500
#define PROTO_HAS_NRF24L01
//Ensure functions are loaded for protocol modules
void SPI_ProtoInit(void);
void SPI_AVRProgramInit(void);
int SPI_ConfigSwitch(unsigned csn_high, unsigned csn_low);
int SPI_ProtoGetPinConfig(int module, int state);
uint32_t AVR_StartProgram(void);
int AVR_Erase(void);
int AVR_Program(uint32_t address, uint8_t *data, int pagesize);
int AVR_SetFuses(void);
int AVR_ResetFuses(void);
int AVR_VerifyFuses(void);
int AVR_Verify(uint8_t *data, int size);

struct mcu_pin;
void MCU_InitModules(void);
int MCU_SetPin(struct mcu_pin *, const char *name);
const char *MCU_GetPinName(char *str, struct mcu_pin *);
void MCU_SerialNumber(uint8_t *var, int len);
#ifdef MODULAR
  #define MODULE_CALLTYPE __attribute__((__long_call__))
#else
  #define MODULE_CALLTYPE
#endif

#endif

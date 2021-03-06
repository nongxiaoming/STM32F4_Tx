#ifndef __DRV_LCD_H_
#define __DRV_LCD_H

#include <stm32f4xx.h>

/* LCD is connected to the FSMC_Bank1_NOR/SRAM1 and NE1 is used as ship select signal */
#define LCD_DATA              (*((volatile unsigned short *) 0x60020000))  /* A16 RS = 0 */
#define LCD_REG               (*((volatile unsigned short *) 0x60000000))  /* A16 RS = 1 */

#define LCD_WIDTH       480                 /* Screen Width (in pixels)           */
#define LCD_HEIGHT      320                 /* Screen Hight (in pixels)           */

#define NOP                             0x00
#define SOFT_RESET                      0x01
#define GET_RED_CHANNEL                 0x06
#define GET_GREEN_CHANNEL               0x07
#define GET_BLUE_CHANNEL                0x08
#define GET_POWER_MODE                  0x0A
#define GET_ADDRESS_MODE                0x0B
#define GET_PIXEL_FORMAT                0x0C
#define GET_DISPLAY_MODE                0x0D
#define GET_SIGNAL_MODE                 0x0E
#define GET_DIAGNOSTIC_RESULT           0x0F
#define ENTER_SLEEP_MODE                0x10
#define EXIT_SLEEP_MODE                 0x11
#define ENTER_PARTIAL_MODE              0x12
#define ENTER_NORMAL_MODE               0x13
#define EXIT_INVERT_MODE                0x20
#define ENTER_INVERT_MODE               0x21
#define SET_GAMMA_CURVE                 0x26
#define SET_DISPLAY_OFF                 0x28
#define SET_DISPLAY_ON                  0x29
#define SET_COLUMN_ADDRESS              0x2A
#define SET_PAGE_ADDRESS                0x2B
#define WRITE_MEMORY_START              0x2C
#define WITE_LUT                        0x2D
#define READ_MEMORY_START               0x2E
#define SET_PARTIAL_AREA                0x30
#define SET_SCROLL_AREA                 0x33
#define SET_TEAR_OFF                    0x34
#define SET_TEAR_ON                     0x35
#define SET_ADDRESS_MODE                0x36
#define SET_SCROLL_START                0x37
#define EXIT_IDLE_MODE                  0x38
#define ENTER_IDLE_MODE                 0x39
#define SET_PIXEL_FORMAT                0x3A
#define WRITE_MEMORY_CONTINUE           0x3C
#define READ_MEMORY_CONTINUE            0x3E
#define SET_TEAR_SCANLINE               0x44
#define GET_SCANLINE                    0x45
#define READ_DDB_START                  0xA1
#define READ_DDB_CONTINUE               0xA8

#define COMMAND_ACCESS_PROTECT          0xB0
#define LOW_POWER_MODE_CONTROL          0xB1
#define FRAME_MEMORY_ACCESS_AND_INTERFACE_SETTING           0xB3
#define DISPLAY_MODE_AND_FRAME_MEMORY_WRITE_MODE_SETTING    0xB4
#define DEVICE_CODE_READ                0xBF
#define PANEL_DRIVING_SETTING           0xC0
#define DISPLAY_TIMING_SETTING_FOR_NORMAL_MODE              0xC1
#define DISPLAY_TIMING_SETTING_FOR_PARTIAL_MODE             0xC2
#define DISPLAY_TIMING_SETTING_FOR_IDLE_MODE                0xC3
#define FRAME_RATE_AND_INVERSION_CONTROL                    0xC5
#define INTERFACE_CONTROL               0xC6
#define GAMMA_SETTING                   0xC8
#define POWER_SETTING                   0xD0
#define VCOM_CONTROL                    0xD1
#define POWER_SETTING_FOR_NORMAL_MODE   0xD2
#define POWER_SETTING_FOR_PARTIAL_MODE  0xD3
#define POWER_SETTING_FOR_IDLE_MODE     0xD4
#define NV_MEMORY_WRITE                 0xE0
#define NV_MEMORY_CONTROL               0xE1
#define NV_MEMORY_STATUS                0xE2
#define NV_MEMORY_PROTECTION            0xE3
#define LSI_TEST_REGISTERS              0xB0

#define LCD_ReadRAM_Start()     LCD_WriteCmd(READ_MEMORY_START)
#define LCD_WriteRAM_Start()    LCD_WriteCmd(WRITE_MEMORY_START)

/* LCD primitive functions */
void LCD_Clear(uint16_t color);
void LCD_Init(void);
    /* Strings */
void LCD_PrintCharXY(uint16_t x, uint16_t y, uint32_t c);
void LCD_PrintChar(uint32_t c);
void LCD_PrintStringXY(uint16_t x, uint16_t y, const char *str);
void LCD_PrintString(const char *str);
void LCD_SetXY(uint16_t x,uint16_t y);
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

#endif


#include "drv_lcd.h"
#include "delay.h"
#include <stdint.h>


/**
  * @brief  Configures LCD Control lines (FSMC Pins) in alternate function mode.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
    /*-- GPIOs Configuration -----------------------------------------------------*/
    /*
     +-------------------+--------------------+------------------+--------------+
     +                       LCD pins assignment                                +
     +-------------------+--------------------+------------------+--------------+
     | PD0  <-> LCD_D2  | PE7  <-> LCD_D4  | PB5  <-> LCD_BL |
     | PD1  <-> LCD_D3  | PE8  <-> LCD_D5  |-----------------+
     | PD4  <-> LCD_RD  | PE9  <-> LCD_D6  |
     | PD5  <-> LCD_WR  | PE10 <-> LCD_D7  |
     | PD7  <-> LCD_CS  | PE11 <-> LCD_D8  |
     | PD8  <-> LCD_D13 | PE12 <-> LCD_D9  |
     | PD9  <-> LCD_D14 | PE13 <-> LCD_D10 |
     | PD10 <-> LCD_D15 | PE14 <-> LCD_D11 |
     | PD11 <-> LCD_RS  | PE15 <-> LCD_D12 |
     | PD14 <-> LCD_D0  |------------------+
     | PD15 <-> LCD_D1  |
     +------------------+
    */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE |
                           RCC_AHB1Periph_GPIOF, ENABLE);

    /* SRAM Data lines,  NOE and NWE configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 |
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15 |
                                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

    /* LCD BL lines configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* turn on lcd backlight */
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

/**
  * @brief  Configures the Parallel interface (FSMC) for LCD(Parallel mode)
  * @param  None
  * @retval None
  */
static void FSMC_Configuration(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;

    /* Enable FSMC clock */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

    /* FSMC_Bank1_NORSRAM1 configuration */
    p.FSMC_AddressSetupTime = 3;
    p.FSMC_AddressHoldTime = 0;
    p.FSMC_DataSetupTime = 4;
    p.FSMC_BusTurnAroundDuration = 0;
    p.FSMC_CLKDivision = 0;
    p.FSMC_DataLatency = 0;
    p.FSMC_AccessMode = FSMC_AccessMode_B;
    /* Color LCD configuration ------------------------------------
       LCD configured as follow:
          - Data/Address MUX = Disable
          - Memory Type = SRAM
          - Data Width = 16bit
          - Write Operation = Enable
          - Extended Mode = Enable
          - Asynchronous Wait = Disable */

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

    /* Enable FSMC NOR/SRAM Bank1 */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

__inline void LCD_WriteCmd(uint16_t reg)
{
    LCD_REG = reg;
}

__inline void LCD_WriteData(uint16_t value)
{
    LCD_DATA = value;
}

__inline uint16_t LCD_ReadData(void)
{
    return LCD_DATA;
}

static void LCD_SetCursor(uint16_t x, uint16_t y)
{
    //竖屏
//        LCD_WriteCmd (0x2A);//设定x坐标
//        LCD_WriteData(x>>8);
//        LCD_WriteData(0xFF&x);

//        LCD_WriteCmd (0x2B);//设定y坐标
//        LCD_WriteData(y>>8);
//        LCD_WriteData(0xFF&y);

    //横屏
    LCD_WriteCmd(0x2B); //设定x坐标
    LCD_WriteData((LCD_WIDTH - x) >> 8);
    LCD_WriteData(0xFF & (LCD_WIDTH - x));

    LCD_WriteCmd(0x2A); //设定y坐标
    LCD_WriteData(y >> 8);
    LCD_WriteData(0xFF & y);
}

void LCD_DrawPixel(unsigned int color)
{
    LCD_DATA = color;
}

void LCD_DrawPixelXY(unsigned int x, unsigned int y, unsigned int color)
{
    LCD_SetCursor(x, y);
    LCD_WriteRAM_Start();
    LCD_DATA = color;
}

void LCD_DrawStop(void)
{
    return;
}

void LCD_Sleep(void)
{

}
void LCD_Clear(uint16_t color)
{
    uint32_t index;

    LCD_SetCursor(0, 0);
    LCD_WriteRAM_Start();

    for (index = 0; index < 345600; index++)
    {
        LCD_WriteData(color);
    }
}
void LCD_Init(void)
{
    GPIO_Configuration();
    FSMC_Configuration();
    LCD_WriteCmd(SOFT_RESET);
    delay_ms(20);

    LCD_WriteCmd(EXIT_SLEEP_MODE);
    delay_ms(20);

    LCD_WriteCmd(ENTER_NORMAL_MODE);

    LCD_WriteCmd(POWER_SETTING);
    LCD_WriteData(0x07);
    LCD_WriteData(0x41);
    LCD_WriteData(0x1d);
    delay_ms(20);

    LCD_WriteCmd(VCOM_CONTROL);
    LCD_WriteData(0x00);
    LCD_WriteData(0x1c);
    LCD_WriteData(0x1f);

    LCD_WriteCmd(POWER_SETTING_FOR_NORMAL_MODE);
    LCD_WriteData(0x0001);
    LCD_WriteData(0x0011);

    LCD_WriteCmd(PANEL_DRIVING_SETTING);
    LCD_WriteData(0x00);       // 设置扫描模式
    LCD_WriteData(0x3b);       // 设定行数480行
    LCD_WriteData(0x00);
    LCD_WriteData(0x02);       // 5frames
    LCD_WriteData(0x11);

    LCD_WriteCmd(DISPLAY_TIMING_SETTING_FOR_NORMAL_MODE);
    LCD_WriteData(0x10);
    LCD_WriteData(0x0b);
    LCD_WriteData(0x88);

    LCD_WriteCmd(FRAME_RATE_AND_INVERSION_CONTROL);
    LCD_WriteData(0x01);       // 100hz

    LCD_WriteCmd(GAMMA_SETTING);
    LCD_WriteData(0x00);
    LCD_WriteData(0x26);
    LCD_WriteData(0x21);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x1f);
    LCD_WriteData(0x65);
    LCD_WriteData(0x23);
    LCD_WriteData(0x77);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0f);
    LCD_WriteData(0x00);

    LCD_WriteCmd(SET_ADDRESS_MODE);
    LCD_WriteData(0x0a);
    LCD_WriteCmd(SET_PIXEL_FORMAT);
    LCD_WriteData(0x55);

    LCD_WriteCmd(SET_DISPLAY_ON);

    LCD_WriteCmd(WRITE_MEMORY_START);
    delay_ms(20);

}


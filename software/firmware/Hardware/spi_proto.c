/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Deviation is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Deviation.  If not, see <http://www.gnu.org/licenses/>.
 */

/*The following will force the loading of various
  functions used in the protocol modules, but unused elsewhere
  in Deviation.
  Note that we lie aboiut the arguments to these functions. It is
  Important that the actual functions never execute
*/
#include "stm32f4xx.h"
#include "common.h"
#include "devo.h"
#include "tx.h"
#include "interface.h"
#include <stdlib.h>

static void spi_set_baudrate_prescaler(SPI_TypeDef* SPIx, uint16_t baudrate)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(baudrate));
	SPIx->CR1&=0xffc7;
	SPIx->CR1 |= baudrate;
}
uint16_t spi_xfer(SPI_TypeDef* SPIx, uint16_t data)
{
 /* Wait until the TXE goes high */
 while (!(SPIx->SR & SPI_I2S_FLAG_TXE));
 /* Start the transfer */
 SPIx->DR = data;
 /* Wait until there is a byte to read */
 while (!(SPIx->SR & SPI_I2S_FLAG_RXNE)) ;
 return SPIx->DR;
}
#if HAS_MULTIMOD_SUPPORT
int SPI_ConfigSwitch(unsigned csn_high, unsigned csn_low)
{
    int i;
	  GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
    //Switch output on clock before switching off SPI
    //Otherwise the pin will float which could cause a false trigger
    //SCK is now on output
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
    SPI_Cmd(SPI2,DISABLE);
    for(i = 0; i < 100; i++)
        asm volatile ("nop");
    GPIO_SetBits(GPIOB, GPIO_Pin_13);
    for(i = 0; i < 100; i++)
        asm volatile ("nop");
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
    //Switch back to SPI
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
    spi_set_baudrate_prescaler(SPI2, SPI_BaudRatePrescaler_128);
    SPI_Cmd(SPI2,ENABLE);
    //Finally ready to send a command
    
    int byte1 = spi_xfer(SPI2, csn_high); //Set all other pins high
    int byte2 = spi_xfer(SPI2, csn_low); //Toggle related pin with CSN
    for(i = 0; i < 100; i++)
        asm volatile ("nop");
    //Reset transfer speed
		SPI_Cmd(SPI2,DISABLE);
    spi_set_baudrate_prescaler(SPI2, SPI_BaudRatePrescaler_16);
    SPI_Cmd(SPI2,ENABLE);
    return byte1 == 0xa5 ? byte2 : 0;
}

int SPI_ProtoGetPinConfig(int module, int state) {
    if (module >= TX_MODULE_LAST || Transmitter.module_enable[module].port != SWITCH_ADDRESS)
        return 0;
    if(state == CSN_PIN)
        return 1 << (Transmitter.module_enable[module].pin & 0x0F);
    if(state == ENABLED_PIN) {
        if(module == NRF24L01) {
            return 1 << ((Transmitter.module_enable[module].pin >> 8) & 0x0F);
        }
        return 0;
    }
    if(state == DISABLED_PIN) {
        return 0;
    }
    /*
    if(state == RESET_PIN) {
        if (module == CYRF6936)
            return 1 << ((Transmitter.module_enable[module].pin >> 8) & 0x0F);
        return 0;
    }
    */
    return 0;
}
#endif

void SPI_ProtoInit()
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  SPI_InitTypeDef  SPI_InitStructure;
	
    /* Enable SPI2 */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	  
    /* Enable GPIOA */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
    /* Enable GPIOB */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);

    /* SCK, MOSI, MISO */

	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*CYRF cfg */
    /* Reset and CS */
   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_11);

#if 0 //In power.c
    //Disable JTAG and SWD and set both pins high
    AFIO_MAPR = (AFIO_MAPR & ~AFIO_MAPR_SWJ_MASK) | AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF;
    gpio_set_mode(GPIO_BANK_JTMS_SWDIO, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO_JTMS_SWDIO);
    gpio_set(GPIO_BANK_JTMS_SWDIO, GPIO_JTMS_SWDIO);
    gpio_set_mode(GPIO_BANK_JTCK_SWCLK, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO_JTCK_SWCLK);
    gpio_set(GPIO_BANK_JTCK_SWCLK, GPIO_JTCK_SWCLK);
#endif

#if HAS_MULTIMOD_SUPPORT
    if(Transmitter.module_enable[MULTIMOD].port) {
        struct mcu_pin *port = &Transmitter.module_enable[MULTIMOD];
        printf("Switch port: %08x pin: %04x\n", port->port, port->pin);
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
				GPIO_InitStructure.GPIO_Pin = port->pin;
				GPIO_Init(port->port, &GPIO_InitStructure);
        GPIO_SetBits(port->port, port->pin);
    }
#endif //HAS_MULTIMOD_SUPPORT
    for (int i = 0; i < MULTIMOD; i++) {
        if(Transmitter.module_enable[i].port
           && Transmitter.module_enable[i].port != SWITCH_ADDRESS)
        {
            struct mcu_pin *port = &Transmitter.module_enable[i];
            printf("%s port: %08x pin: %04x\n", MODULE_NAME[i], port->port, port->pin);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
						GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
						GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
						GPIO_InitStructure.GPIO_Pin = port->pin;
						GPIO_Init(port->port, &GPIO_InitStructure);
						GPIO_SetBits(port->port, port->pin);
        }
    }
    /* Includes enable? */

		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
		
    SPI_SSOutputCmd(SPI2,DISABLE);
    SPI_Cmd(SPI2,ENABLE);

    PROTO_Stubs(0);
}

//void SPI_AVRProgramInit()
//{
//    rcc_set_ppre1(RCC_CFGR_PPRE1_HCLK_DIV16);  //72 / 16 = 4.5MHz
//    SPI_Cmd(SPI2,DISABLE);
//    spi_set_baudrate_prescaler(SPI2, SPI_CR1_BR_FPCLK_DIV_256);// 4.5 / 256 = 17.5kHz
//    SPI_Cmd(SPI2,ENABLE);
//}

void MCU_InitModules()
{
    //CSN
	  GPIO_InitTypeDef GPIO_InitStructure;
	 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    Transmitter.module_enable[CYRF6936].port = GPIOB;
    Transmitter.module_enable[CYRF6936].pin = GPIO_Pin_12;
    Transmitter.module_poweramp = (1 << CYRF6936);
}

int MCU_SetPin(struct mcu_pin *port, const char *name) {
    switch(name[0]) {
        case 'A':
        case 'a':
            port->port = GPIOA; break;
        case 'B':
        case 'b':
            port->port = GPIOB; break;
        case 'C':
        case 'c':
            port->port = GPIOC; break;
        case 'D':
        case 'd':
            port->port = GPIOD; break;
        case 'E':
        case 'e':
            port->port = GPIOE; break;
        case 'F':
        case 'f':
            port->port = GPIOF; break;
        case 'G':
        case 'g':
            port->port = GPIOG; break;
#if HAS_MULTIMOD_SUPPORT
        case 'S':
        case 's':
            port->port = (GPIO_TypeDef*)SWITCH_ADDRESS; break;
#endif
        default:
            port->port = 0;
            port->pin = 0;
            if(strcasecmp(name, "None") == 0) {
                return 1;
            }
            return 0;
    }
    if (port->port != (GPIO_TypeDef*)SWITCH_ADDRESS) {
        int x = atoi(name+1);
        if (x > 15)
            return 0;
         port->pin = 1 << x;
    } else {
        port->pin = strtol(name+1,NULL, 16);
    }
    printf("port: %08x pin: %04x\n", port->port, port->pin);
    return 1;
}

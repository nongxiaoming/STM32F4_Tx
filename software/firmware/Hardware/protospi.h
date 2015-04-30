#ifndef __SPIPROTO_H_
#define __SPIPROTO_H_
#include "stm32f4xx.h"
#include "tx.h"

static inline uint8_t PROTOSPI_read3wire(){
    uint8_t data;
    while(!(SPI2->SR & SPI_SR_TXE))
        ;
    while(SPI2->SR & SPI_SR_BSY)
        ;

    SPI_Cmd(SPI2,DISABLE);
    spi_set_bidirectional_receive_only_mode(SPI2);
    /* Force read from SPI_DR to ensure RXNE is clear (probably not needed) */
    volatile uint8_t x = SPI2->DR;
    (void)x;
    SPI_Cmd(SPI2,ENABLE);  //This starts the data read
    //Wait > 1 SPI clock (but less than 8).  clock is 4.5MHz
    asm volatile ("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\t"
                  "nop\n\tnop\n\tnop\n\tnop\n\tnop\n\t"
                  "nop\n\tnop\n\tnop\n\tnop\n\tnop\n\t"
                  "nop\n\tnop\n\tnop\n\tnop\n\tnop");
    SPI_Cmd(SPI2,DISABLE); //This ends the read window
    data = spi_read(SPI2);
    spi_set_unidirectional_mode(SPI2);
    SPI_Cmd(SPI2,ENABLE);
    return data;
}

#define PROTOSPI_pin_set(io)   GPIO_SetBits(io.port,io.pin)
#define PROTOSPI_pin_clear(io) GPIO_ResetBits(io.port,io.pin)
#define PROTOSPI_xfer(byte)   spi_xfer(SPI2, byte)
#define _NOP()  asm volatile ("nop")

static const struct mcu_pin CYRF_RESET_PIN ={GPIOB, GPIO_Pin_11};
//static const struct mcu_pin AVR_RESET_PIN ={GPIO_BANK_JTCK_SWCLK, GPIO_JTCK_SWCLK};

#endif // _SPIPROTO_H_

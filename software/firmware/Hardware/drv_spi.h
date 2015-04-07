#ifndef __DRV_SPI_H
#define __DRV_SPI_H

#include "stm32f4xx.h"

int SPI_HW_Init(void);
int32_t SPI_TransferByte(SPI_TypeDef *SPIx, uint8_t byte);
int32_t SPI_TransferBlock(SPI_TypeDef *SPIx,uint8_t *send_buffer, uint8_t *receive_buffer, uint16_t len);

#endif // __DRV_SPI_H

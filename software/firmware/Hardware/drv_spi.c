#include "drv_spi.h"

#define SPI1_CLOCK		        RCC_APB2Periph_SPI1
#define SPI1_AF		            GPIO_AF_SPI1
#define SPI1_GPIO_CLOCK       RCC_AHB1Periph_GPIOA
#define SPI1_SCK_PORT	        GPIOA
#define SPI1_MISO_PORT	      GPIOA
#define SPI1_MOSI_PORT	      GPIOA
#define SPI1_SCK_PIN	        GPIO_Pin_5
#define SPI1_MISO_PIN	        GPIO_Pin_6
#define SPI1_MOSI_PIN	        GPIO_Pin_7
#define SPI1_SCK_SOURCE	      GPIO_PinSource5
#define SPI1_MISO_SOURCE	    GPIO_PinSource6
#define SPI1_MOSI_SOURCE	    GPIO_PinSource7




static void RCC_Configuration(void)
{
    /* Enable GPIOA clocks */
    RCC_AHB1PeriphClockCmd(SPI1_GPIO_CLOCK, ENABLE);
    /* Enable SPI1 clock */
    RCC_APB2PeriphClockCmd(SPI1_CLOCK, ENABLE);
}

static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    /* Configure SPI1 SCK PIN */
    GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN;
    GPIO_Init(SPI1_SCK_PORT, &GPIO_InitStructure);
	  /* Connect alternate function */
    GPIO_PinAFConfig(SPI1_SCK_PORT, SPI1_SCK_SOURCE, SPI1_AF);
	
	  /* Configure SPI1 MISO PIN */
    GPIO_InitStructure.GPIO_Pin = SPI1_MISO_PIN;
    GPIO_Init(SPI1_MISO_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(SPI1_MISO_PORT, SPI1_MISO_SOURCE, SPI1_AF);
	
	  /* Configure SPI1 MOSI PIN */
    GPIO_InitStructure.GPIO_Pin = SPI1_MOSI_PIN;
    GPIO_Init(SPI1_MOSI_PORT, &GPIO_InitStructure);
	  GPIO_PinAFConfig(SPI1_MOSI_PORT, SPI1_MOSI_SOURCE, SPI1_AF);

}
static void SPI_Configuration(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
/* SPI1 configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable the SPI  */
  SPI_Cmd(SPI1, ENABLE);
	
}

int32_t SPI_SetClockSpeed(SPI_TypeDef *SPIx, uint32_t spi_speed)
{

	SPI_InitTypeDef SPI_InitStructure;

	uint16_t spi_prescaler;

	//SPI clock is different depending on the bus
	uint32_t spiBusClock = 0;
  
	/* Disable the SPI  */
  SPI_Cmd(SPIx, DISABLE);
	
	if(SPIx == SPI1)
		spiBusClock = SystemCoreClock / 2;
	else
		spiBusClock = SystemCoreClock / 4;

	//The needed prescaler for desired speed
	float desiredPrescaler=(float) spiBusClock / spi_speed;

	//Choosing the existing prescaler nearest the desiredPrescaler
	if(desiredPrescaler <= 2)
		spi_prescaler = SPI_BaudRatePrescaler_2;
	else if(desiredPrescaler <= 4)
		spi_prescaler = SPI_BaudRatePrescaler_4;
	else if(desiredPrescaler <= 8)
		spi_prescaler = SPI_BaudRatePrescaler_8;
	else if(desiredPrescaler <= 16)
		spi_prescaler = SPI_BaudRatePrescaler_16;
	else if(desiredPrescaler <= 32)
		spi_prescaler = SPI_BaudRatePrescaler_32;
	else if(desiredPrescaler <= 64)
		spi_prescaler = SPI_BaudRatePrescaler_64;
	else if(desiredPrescaler <= 128)
		spi_prescaler = SPI_BaudRatePrescaler_128;
	else
		spi_prescaler = SPI_BaudRatePrescaler_256;

	/* SPI configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;

	/* Adjust the prescaler for the peripheral's clock */
	SPI_InitStructure.SPI_BaudRatePrescaler = spi_prescaler;

	/* Write back the new configuration */
	SPI_Init(SPIx, &SPI_InitStructure);

  /* Enable the SPI  */
  SPI_Cmd(SPIx, ENABLE);
	
	//return set speed
	return spiBusClock / spi_prescaler;
}
void spi_set_baudrate_prescaler(SPI_TypeDef* SPIx, uint16_t baudrate)
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
int SPI_HW_Init(void)
{
    RCC_Configuration();
    GPIO_Configuration();
    SPI_Configuration();
	
    return 0;
}

int32_t SPI_TransferByte(SPI_TypeDef *SPIx, uint8_t byte)
{

  /* Wait until the TXE goes high */
	while (!(SPIx->SR & SPI_I2S_FLAG_TXE));

  /* Send byte through the SPI1 peripheral */
  SPIx->DR = byte;
	
  /* Wait until there is a byte to read */
  while (!(SPIx->SR & SPI_I2S_FLAG_RXNE));
	
  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPIx);
}


int32_t SPI_TransferBlock(SPI_TypeDef *SPIx,uint8_t *send_buffer, uint8_t *receive_buffer, uint16_t len)
{

	uint8_t byte;

	while (len--) {
    
	  /* Wait until the TXE goes high */
		while (!(SPIx->SR & SPI_I2S_FLAG_TXE));
		
		/* Start the transfer */
		SPIx->DR = send_buffer ? *(send_buffer++) : 0xff;

		/* Wait until there is a byte to read */
		while (!(SPIx->SR & SPI_I2S_FLAG_RXNE)) ;

		/* Read the rx'd byte */
		byte = SPIx->DR;

		/* save the received byte */
		if (receive_buffer)
			*(receive_buffer++) = byte;


	}

	/* Wait for SPI transfer to have fully completed */
	while (SPIx->SR & SPI_I2S_FLAG_BSY) ;

	return 0;
}

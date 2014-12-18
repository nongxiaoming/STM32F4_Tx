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
#include "stm32f4xx.h"
#include "common.h"

#define SPI_FLASH_PageSize              256                          //页大小
#define SPI_FLASH_PerWritePageSize      256                          //每页大小

/*W25X指令定义*/
#define W25X_WriteEnable		            0x06                            //写使能指令
#define W25X_WriteDisable		            0x04                            //写失能指令
#define W25X_ReadStatusReg		          0x05                            //读状态寄存器
#define W25X_WriteStatusReg		          0x01                            //写状态寄存器
#define W25X_ReadData			              0x03                            //读数据指令
#define W25X_FastReadData		            0x0B                            //快速读模式指令
#define W25X_FastReadDual		            0x3B                            //快读双输出指令    
#define W25X_PageProgram		            0x02                            //页写入指令
#define W25X_BlockErase			            0xD8                            //块擦除指令
#define W25X_SectorErase		            0x20                            //扇区擦除指令
#define W25X_ChipErase			            0xC7                            //片擦除指令
#define W25X_PowerDown			            0xB9                            //进入掉电模式指令
#define W25X_ReleasePowerDown	          0xAB                            //退出掉电模式
#define W25X_DeviceID			              0xAB                            //读取芯片ID
#define W25X_ManufactDeviceID        	  0x90                            //读取制造ID
#define W25X_JedecDeviceID		          0x9F 

#define WIP_Flag                        0x01                            //写入忙标志位
#define Dummy_Byte                      0xFF                            //空数据

#define SPIFLASH_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)       //使能片选
#define SPIFLASH_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)         //失能片选


/**
* SPIFLASH_CS    <--->  PA4
* SPIFLASH_MOSI  <--->  PA7
* SPIFLASH_MISO  <--->  PA6
* SPIFLASH_CLK   <--->  PA5
**/
/******************************************************************************************
*函数名：SPI_FLASH_Init()
* 参数：void
* 返回值：void
* 功能：SPIFLASH初始化函数，外部调用
*********************************************************************************************/
void SPIFLASH_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  SPIFLASH_CS_HIGH();
	
  	SPI_I2S_DeInit(SPI1);
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;           //全双工
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                            //8位数据模式
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                                    //空闲模式下SCK为1
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                                  //数据采样从第2个时间边沿开始
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                                     //NSS软件管理
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;           //波特率
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                             //大端模式
  	SPI_InitStructure.SPI_CRCPolynomial = 7;                                       //CRC多项式
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                                  //主机模式
  	SPI_Init(SPI1, &SPI_InitStructure);
  	SPI_Cmd(SPI1, ENABLE);
}

/******************************************************************************************
*函数名：SPI_FLASH_SendByte()
* 参数：u8 byte        写入的数据
* 返回值：u8 8位数据
* 功能：SPIFLASH读写一个字节函数，外部调用
*********************************************************************************************/
static u8 SPIFLASH_SendByte(u8 byte)
{
  /*等待SPI发送空闲*/
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  /*发送8位数据 */
  SPI_I2S_SendData(SPI1, byte);
 /* 等待接收8位数据 */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  /* 返回接收的8位数据 */
  return SPI_I2S_ReceiveData(SPI1);
}

/******************************************************************************************
*函数名：SPIFLASH_WriteEnable()
* 参数：void
* 返回值：void
* 功能：SPIFLASH写使能函数，外部调用
*********************************************************************************************/
static void SPIFLASH_WriteEnable(void)
{
   /* 使能片选 */
  SPIFLASH_CS_LOW();
  /*发送写使能指令*/
  SPIFLASH_SendByte(W25X_WriteEnable);
  /*失能片选*/
  SPIFLASH_CS_HIGH();
}
/******************************************************************************************
*函数名：SPIFLASH_WriteDisable()
* 参数：void
* 返回值：void
* 功能：SPIFLASH写使能函数，外部调用
*********************************************************************************************/
static void SPIFLASH_WriteDisable(void)
{
   /* 使能片选 */
  SPIFLASH_CS_LOW();
  /*发送写使能指令*/
  SPIFLASH_SendByte(W25X_WriteDisable);
  /*失能片选*/
  SPIFLASH_CS_HIGH();
}

/******************************************************************************************
*函数名：SPI_FLASH_ReadDeviceID()
* 参数：void
* 返回值：u32 设备ID
* 功能：SPIFLASH读取设备ID函数，外部调用
*********************************************************************************************/
u32 SPIFLASH_ReadDeviceID(void)
{
  u32 Temp = 0;
   /* 使能片选 */
  SPIFLASH_CS_LOW();
  /*发送读取ID指令*/
  SPIFLASH_SendByte(W25X_DeviceID);
  SPIFLASH_SendByte(Dummy_Byte);
  SPIFLASH_SendByte(Dummy_Byte);
  SPIFLASH_SendByte(Dummy_Byte);
  /*读取8位数据*/
  Temp = SPIFLASH_SendByte(Dummy_Byte);
  /*失能片选*/
  SPIFLASH_CS_HIGH();
  return Temp;
}

/******************************************************************************************
*函数名：SPI_FLASH_WaitForWriteEnd()
* 参数：void
* 返回值：void
* 功能：SPIFLASH等待写完毕函数，外部调用
*********************************************************************************************/
void SPIFLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;
   /* 使能片选 */
  SPIFLASH_CS_LOW();
  /*发送读状态指令 */
  SPIFLASH_SendByte(W25X_ReadStatusReg);
  /*循环发送空数据直到FLASH芯片空闲*/
  do
  {
    /* 发送空字节 */
    FLASH_Status = SPIFLASH_SendByte(Dummy_Byte);
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* 检测是否空闲*/
  /*失能片选*/
  SPIFLASH_CS_HIGH();
}

/******************************************************************************************
*函数名：SPI_Flash_PowerDown()
* 参数：void
* 返回值：void
* 功能：SPIFLASH进入掉电模式函数，外部调用
*********************************************************************************************/
void SPIFlash_PowerDown(void)   
{ 
  /* 使能片选 */
  SPIFLASH_CS_LOW();
  /*发送掉电指令 */
  SPIFLASH_SendByte(W25X_PowerDown);
  /*失能片选*/
  SPIFLASH_CS_HIGH();
}   

/******************************************************************************************
*函数名：SPI_Flash_WAKEUP()
* 参数：void
* 返回值：void
* 功能：SPIFLASH唤醒掉电模式函数，外部调用
*********************************************************************************************/
void SPIFlash_WAKEUP(void)   
{
  /* 使能片选 */
  SPIFLASH_CS_LOW();
  /* 发送退出掉电模式指令 */
  SPIFLASH_SendByte(W25X_ReleasePowerDown);
  /*失能片选*/
  SPIFLASH_CS_HIGH();              
}  

/******************************************************************************************
*函数名：SPI_FLASH_SectorErase()
* 参数：u32 SectorAddr   块地址
* 返回值：void
* 功能：SPIFLASH扇区擦除函数，外部调用
*********************************************************************************************/
void SPIFLASH_SectorErase(u32 SectorAddr)
{
  /*发送写数据使能指令*/
  SPIFLASH_WriteEnable();
	/*等待数据写完，保证写操作是空闲的*/
  SPIFLASH_WaitForWriteEnd();
  /* 使能片选 */
  SPIFLASH_CS_LOW();
  /*发送扇区擦除指令*/
  SPIFLASH_SendByte(W25X_SectorErase);
  /*发送块地址高8位*/
  SPIFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*发送块地址中8位*/
  SPIFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /*发送块地址低8位*/
  SPIFLASH_SendByte(SectorAddr & 0xFF);
  /*失能片选*/
  SPIFLASH_CS_HIGH();
  /* 等待写完毕*/
  SPIFLASH_WaitForWriteEnd();
}

/******************************************************************************************
*函数名：SPI_FLASH_BulkErase()
* 参数：void
* 返回值：void
* 功能：SPIFLASH整片擦除函数，外部调用
*********************************************************************************************/
void SPIFLASH_BulkErase(void)
{
  /*使能写入*/
  SPIFLASH_WriteEnable();
   /* 使能片选 */
  SPIFLASH_CS_LOW();
  /*发送整片擦除指令*/
  SPIFLASH_SendByte(W25X_ChipErase);
  /*失能片选*/
  SPIFLASH_CS_HIGH();
  /* 等待写完成*/
  SPIFLASH_WaitForWriteEnd();
}

/******************************************************************************************
*函数名：SPI_FLASH_PageWrite()
* 参数：u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite 数据指针，写入地址，写入的个数
* 返回值：void
* 功能：SPIFLASH页写入数据函数，外部调用
*********************************************************************************************/
void SPIFLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
   /*使能写入*/
  SPIFLASH_WriteEnable();
  /*使能片选*/
  SPIFLASH_CS_LOW();
  /* 发送页写入指令*/
  SPIFLASH_SendByte(W25X_PageProgram);
  /*发送高8位数据地址*/
  SPIFLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*发送中8位数据地址*/
  SPIFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*发送低8位数据地址*/
  SPIFLASH_SendByte(WriteAddr & 0xFF);
  /*检测写入的数据是否超出页的容量大小*/
  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
  }
  /*循环写入数据*/
  while (NumByteToWrite--)
  {
    /*发送数据*/
    SPIFLASH_SendByte(*pBuffer);
    /* 指针移到下一个写入数据 */
    pBuffer++;
  }
  /*失能片选*/
  SPIFLASH_CS_HIGH();
  /* 等待写完成*/
  SPIFLASH_WaitForWriteEnd();
}

/******************************************************************************************
*函数名：SPI_FLASH_BufferWrite()
* 参数：u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite 数据指针，写入地址，写入的个数
* 返回值：void
* 功能：SPIFLASH多个数据函数，外部调用
*********************************************************************************************/
void SPIFLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
  Addr = WriteAddr % SPI_FLASH_PageSize;                           //计算写入的页的对应初始地址
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;                //计算总共要写的页数
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;               //计算剩余单个页写的数据个数
  if (Addr == 0) /* 如果要写入的页地址为0，说明正好整页写数据，没有偏移*/
  {
    if (NumOfPage == 0) /* 如果计算的写入页数为0，说明数据量在一个页的范围内，可直接进行页的写*/
    {
      SPIFLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);      //进行页写数据
    }
    else /* 如果要写的页数大于0*/
    { 
			/*先将开头数据进行整页写入*/
      while (NumOfPage--)
      { 
				//整页写入
        SPIFLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				//地址偏移
        WriteAddr +=  SPI_FLASH_PageSize;
				//数据指针偏移
        pBuffer += SPI_FLASH_PageSize;
      }
       //将剩余数据个数写入
      SPIFLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /*如果写入的地址不在页的开头位置*/
  {
    if (NumOfPage == 0) /*如果写入数据页的个数为0，即数据小于一页容量*/
    {
      if (NumOfSingle > count) /*如果剩余数据大于当前页的剩余容量*/
      {
        temp = NumOfSingle - count;     //计算超出的数据个数
				/*写满当前页*/
        SPIFLASH_PageWrite(pBuffer, WriteAddr, count);
				/*设置地址偏移*/
        WriteAddr +=  count;
				/*设置数据指针偏移*/
        pBuffer += count;
        /*将剩余量写入新的页*/
        SPIFLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else  /*如果剩余数据小于当前页的剩余容量*/
      {
				/*直接写入当前页*/
        SPIFLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /*如果写入数据页的个数大于0，即数据大于一页容量*/
    {
      NumByteToWrite -= count;         //总数据减去当前页剩余的容量
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;  //计算要写的整页个数
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize; //计算剩余数据个数
      /*将开头数据写入当前页剩余字节个数*/
      SPIFLASH_PageWrite(pBuffer, WriteAddr, count);
			/*设置地址偏移*/
      WriteAddr +=  count;
			/*设置数据指针偏移*/
      pBuffer += count;
       /*开始剩下数据的整页写入*/
      while (NumOfPage--)
      {
				/*写入一个页的字节数*/
        SPIFLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				/*设置地址偏移*/
        WriteAddr +=  SPI_FLASH_PageSize;
				/*设置指针偏移*/
        pBuffer += SPI_FLASH_PageSize;
      }
      /*如果剩余数据大于0，将剩余的个数写入下一个页*/
      if (NumOfSingle != 0)
      {
        SPIFLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/******************************************************************************************
*函数名：SPI_FLASH_BufferRead()
* 参数：u8* pBuffer, u32 ReadAddr, u16 NumByteToRead 数据指针，读出的地址，读出的个数
* 返回值：void
* 功能：SPIFLASH多个数据函数，外部调用
*********************************************************************************************/
void SPIFLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
   /* 使能片选 */
  SPIFLASH_CS_LOW();
  /*发送读数据指令*/
  SPIFLASH_SendByte(W25X_ReadData);
  /*发送24位数据地址*/
  /* 发送高8位数据地址*/
  SPIFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*发送中8位数据地址*/
  SPIFLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /*发送低8位数据地址*/
  SPIFLASH_SendByte(ReadAddr & 0xFF);
  while (NumByteToRead--) /* 循环读取数据*/
  {
    /*读取一个字节数据*/
    *pBuffer = SPIFLASH_SendByte(Dummy_Byte);
    /*数据指针加1*/
    pBuffer++;
  }
  /*失能片选*/
  SPIFLASH_CS_HIGH();
}

//int SPIFlash_ReadBytesStopCR(u32 readAddress, u32 length, u8 * buffer)
//{
//    u32 i;
//    SPIFlash_SetAddr(0x03, readAddress);

//    for(i=0;i<length;i++)
//    {
//        buffer[i] = ~spi_xfer(SPI1, 0);
//        if (buffer[i] == '\n') {
//            i++;
//            break;
//        }
//    }

//    CS_HI();
//    return i;
//}

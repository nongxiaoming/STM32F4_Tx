 /******************************************************************************
  * 文件路径  ：   User/main.c 
  * 作者      ：  山岩电子工作室
  * 固件库版本：   V1.0.0
  * 日期      ：   10-October-2011
  * 文件描述  ：   程序入口 main()函数
  ******************************************************************************/
/* -------------------包含的头文件-----------------------------------------------*/
#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"


/*************************************************************************************
  * 函数名称：main()
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：程序主入口main函数
  *************************************************************************************/
int main(void)
{
  SystemInit();			                                   //初始化系统时钟，设置时钟为168Mhz
  LED_GPIO_Conf();										   //初始化LED的GPIO配置
//  USART1_Conf();
//  printf("\r\n欢迎使用山岩电子STM32F407VG开发板！\r\n");
//  printf("\r\n        山岩电子工作室！\r\n");
//    printf("\r\n            -----专业，值得信赖！\r\n");
  while (1)
  {
  	LED0(On);					                           //打开LED0
	delay_nms(1000);									   //延时约1s
	LED0(Off);											   //关闭LED0
	delay_nms(1000);									   //延时约1s
    LED1(On);											   //打开LED1
	delay_nms(1000);									   //延时约1s
	LED1(Off);											   //关闭LED1
	delay_nms(1000);									   //延时约1s
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


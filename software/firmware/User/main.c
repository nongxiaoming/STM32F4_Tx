
#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "drv_lcd.h"

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
  LCD_Init();
  while (1)
  {
	 LCD_Clear(0xf800);
  	LED0(On);					                           //打开LED0
	delay_ms(1000);									   //延时约1s
	LED0(Off);											   //关闭LED0
	delay_ms(1000);									   //延时约1s
		 LCD_Clear(0x001f);
    LED1(On);											   //打开LED1
	delay_ms(1000);									   //延时约1s
	LED1(Off);											   //关闭LED1
	delay_ms(1000);									   //延时约1s
  }
}



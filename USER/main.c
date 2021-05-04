#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
//#include "qspi.h"
#include "mpu.h" 
//#include "string.h"
#include "sdram.h"
//#include "malloc.h"
//#include "w25qxx.h"
//#include "ff.h"
#include "exfuns.h"
//#include "string.h"
#include "usmart.h"
#include "sdmmc_sdcard.h"
//#include "fontupd.h"
//#include "text.h"
//#include "touch.h"
#include "common.h"
#include "test_esp8266.h"
#include "mpu9250.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
/************************************************
 ALIENTEK 阿波罗STM32F767开发板扩展实验13
 ATK-ESP8266 WIFI模块测试实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
    
int main(void)
{
	u8 *hello;
	u8 test_set = 22;
	u8 test_get = 22;
	u8 *test_change_set = "55.00";
	u8 *test_change_get = "66.00";
	unsigned long test_step_get;
	unsigned long test_step_set = 998;
	u8 key,fontok=0; 	
  Write_Through();                //开启强制透写！
  Cache_Enable();                 //打开L1-Cache
  MPU_Memory_Protection();        //保护相关存储区域
  HAL_Init();				        //初始化HAL库
  Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
  delay_init(216);                //延时初始化
  uart_init(115200);              //初始化USART1
	usart3_init(115200);  						//初始化串口3波特率为115200
  usmart_dev.init(108);						//初始化usmart
  LED_Init();                     //初始化LED 
  KEY_Init();                     //初始化按键
  SDRAM_Init();                   //SDRAM初始化
  LCD_Init();                     //LCD初始化
	W25QXX_Init();				    			//初始化W25Q256
	tp_dev.init();									//初始化触摸屏
  my_mem_init(SRAMIN);            //初始化内部内存池
  my_mem_init(SRAMEX);            //初始化外部SDRAM内存池
  my_mem_init(SRAMDTCM);           //初始化内部CCM内存池
  exfuns_init();		            	//为fatfs相关变量申请内存  
  f_mount(fs[0],"0:",1);          //挂载SD卡 
 	f_mount(fs[1],"1:",1);          //挂载SPI FLASH.  
	key=KEY_Scan(0);
	if((key==KEY0_PRES)&&(tp_dev.touchtype&0X80)==0)		//强制校准，电阻屏才需要校准
	{
		LCD_Clear(WHITE);	//清屏
		TP_Adjust();  		//屏幕校准 
		TP_Save_Adjdata();	  
		LCD_Clear(WHITE);	//清屏
	}
	fontok=font_init();		//检查字库是否OK
	if(fontok||key==KEY1_PRES)//需要更新字库				 
	{
		LCD_Clear(WHITE);		   	//清屏
 		POINT_COLOR=RED;			//设置字体为红色	   	   	  
		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32");
		while(SD_Init())			//检测SD卡
		{
			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(60,70,200+60,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(60,70,200,16,16,"SD Card OK");
		LCD_ShowString(60,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16,"0:");//从SD卡更新
		while(key)//更新失败		
		{			 		  
			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
		delay_ms(1500);	
		LCD_Clear(WHITE);//清屏	       
	}
	printf("main函数运行开始\r\n");
	sprintf((char*)hello, "初始化结果：%d", MPU9250_Init());
	printf("%s",(char *)hello);             	//初始化MPU9250
	
	//test_esp8266();
	LCD_Clear(WHITE); //清屏
	//printf("atk_8266_test()函数运行开始\r\n");
 	//atk_8266_test();		//进入ATK_ESP8266测试	 
	
	while(mpu_dmp_init())         
   {   
		LCD_ShowString(30,130,200,16,16,"MPU9250 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
		LED0_Toggle;//DS0闪烁 
   }
	
	test_set = dmp_set_pedometer_step_count(test_step_set);
	sprintf((char*)test_change_set, "%ld", test_step_set);
	printf("test_set：%d\r\n", test_set);
	printf("test_change_set：%s\r\n", test_change_set);
	while(1) {
		delay_ms(1000);
		test_get = dmp_get_pedometer_step_count(&test_step_get);
		sprintf((char*)test_change_get, "%ld", test_step_get);
		printf("test_get：%d\r\n", test_get);
		printf("test_change_get：%s\r\n", test_change_get);
	}
	printf("main函数运行完成\r\n");
}

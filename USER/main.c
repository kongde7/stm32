#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "qspi.h"
#include "mpu.h" 
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "w25qxx.h"
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "usmart.h"
#include "sdmmc_sdcard.h"
#include "fontupd.h"
#include "text.h"
#include "touch.h"
#include "common.h"
/************************************************
 ALIENTEK ������STM32F767��������չʵ��13
 ATK-ESP8266 WIFIģ�����ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 �������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
    
int main(void)
{
	u8 key,fontok=0; 	
  Write_Through();                //����ǿ��͸д��
  Cache_Enable();                 //��L1-Cache
  MPU_Memory_Protection();        //������ش洢����
  HAL_Init();				        //��ʼ��HAL��
  Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
  delay_init(216);                //��ʱ��ʼ��
  uart_init(115200);              //��ʼ��USART1
	usart3_init(115200);  						//��ʼ������3������Ϊ115200
  usmart_dev.init(108);						//��ʼ��usmart
  LED_Init();                     //��ʼ��LED 
  KEY_Init();                     //��ʼ������
  SDRAM_Init();                   //SDRAM��ʼ��
  LCD_Init();                     //LCD��ʼ��
	W25QXX_Init();				    			//��ʼ��W25Q256
	tp_dev.init();									//��ʼ��������
  my_mem_init(SRAMIN);            //��ʼ���ڲ��ڴ��
  my_mem_init(SRAMEX);            //��ʼ���ⲿSDRAM�ڴ��
  my_mem_init(SRAMDTCM);           //��ʼ���ڲ�CCM�ڴ��
  exfuns_init();		            	//Ϊfatfs��ر��������ڴ�  
  f_mount(fs[0],"0:",1);          //����SD�� 
 	f_mount(fs[1],"1:",1);          //����SPI FLASH.  
	key=KEY_Scan(0);
	if((key==KEY0_PRES)&&(tp_dev.touchtype&0X80)==0)		//ǿ��У׼������������ҪУ׼
	{
		LCD_Clear(WHITE);	//����
		TP_Adjust();  		//��ĻУ׼ 
		TP_Save_Adjdata();	  
		LCD_Clear(WHITE);	//����
	}
	fontok=font_init();		//����ֿ��Ƿ�OK
	if(fontok||key==KEY1_PRES)//��Ҫ�����ֿ�				 
	{
		LCD_Clear(WHITE);		   	//����
 		POINT_COLOR=RED;			//��������Ϊ��ɫ	   	   	  
		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32");
		while(SD_Init())			//���SD��
		{
			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(60,70,200+60,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(60,70,200,16,16,"SD Card OK");
		LCD_ShowString(60,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16,"0:");//��SD������
		while(key)//����ʧ��		
		{			 		  
			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
		delay_ms(1500);	
		LCD_Clear(WHITE);//����	       
	}
 	atk_8266_test();		//����ATK_ESP8266����	 
}
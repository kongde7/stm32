#include "common.h"
#include "test_esp8266.h"

//连接端口号:8086,可自行修改为其他端口.
const u8* portnum="8080";		 

//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const u8* wifista_ssid="TP-LINK_669D";			//路由器SSID号
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifista_password="7071727370717273jl"; 	//连接密码

//自定义测试函数
void test_esp8266(void) {
//	u16 rlen=0;
	u8 key;
	u8 timex;
	POINT_COLOR=RED;
	printf("进入自定义test_esp8266函数\r\n");
	Show_Str_Mid(0,30,"ATK-ESP8266 WIFI模块测试",16,240); 
	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
	{
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
		Show_Str(40,55,200,16,"未检测到模块!!!",16,0);
		delay_ms(800);
		LCD_Fill(40,55,200,55+16,WHITE);
		Show_Str(40,55,200,16,"尝试连接模块...",16,0); 
	} 
		while(atk_8266_send_cmd("ATE0","OK",20));//关闭回显
		atk_8266_mtest_ui(32,30);
	//此处为选择STA模式
//	while(1)
//	{
		delay_ms(10); 
		atk_8266_at_response(1);//检查ATK-ESP8266模块发送过来的数据,及时上传给电脑
		key=KEY_Scan(0);
		LCD_Clear(WHITE);
		POINT_COLOR=RED;
		Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240);
		Show_Str_Mid(0,50,"正在配置ATK-ESP8266模块，请稍等...",12,240);
		test_sta();//WIFI STA测试
		atk_8266_mtest_ui(32,30);
		timex=0;
		if((timex%20)==0)LED0_Toggle;//200ms闪烁 
		timex++;	 
//	} 
	printf("运行结束：自定义test_esp8266函数\r\n");
}

//进入STA模式，连接路由器
u8 test_sta(void)
{
	u8 test_ip[16] = "1.15.96.152";
	u8 *test_str;
  u8 netpro=1;	//网络模式
	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP缓存
	u8 *p;
	u16 t=999;		//加速第一次获取链接状态
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//连接状态
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//设置WIFI STA模式
	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP服务器关闭(仅AP模式有效) 
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//连接目标路由器,并且获得IP
PRESTA:
	//不使用按键选择函数
	//netpro|=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[0]);	//选择网络模式
	sprintf((char*)test_str, "%s", &netpro);
	printf(ipbuf);
	printf(test_ip);
	printf(test_str);
	printf("%d",netpro);
	printf("为IP地址\r\n");
	
			//自动选择TCP Client模式
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
			Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240); 
			Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
			//取消为0返回的跳转
			//if(atk_8266_ip_set("WIFI-STA 远端IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
			
			//更改为自定义服务器IP
			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",test_ip,(u8*)portnum);    //配置目标TCP服务器
			//sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器
			while(atk_8266_send_cmd(p,"OK",200))
			{
					LCD_Clear(WHITE);
					POINT_COLOR=RED;
					Show_Str_Mid(0,40,"WK_UP:返回重选",16,240);
					Show_Str(30,80,200,12,"ATK-ESP 连接TCP失败",12,0); //连接失败	 
					key=KEY_Scan(0);
					if(key==WKUP_PRES)goto PRESTA;
			}	
			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传
			
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
			Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240);
			Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);			
			LCD_Fill(30,50,239,50+12,WHITE);			//清除之前的显示
			Show_Str(30,50,200,16,"WK_UP:退出测试  KEY1:发送数据",12,0);
			LCD_Fill(30,80,239,80+12,WHITE);
			atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
			sprintf((char*)p,"IP地址:%s 端口:%s",ipbuf,(u8*)portnum);
			Show_Str(30,65,200,12,p,12,0);				//显示IP地址和端口	
			Show_Str(30,80,200,12,"状态:",12,0); 		//连接状态
			Show_Str(120,80,200,12,"模式:",12,0); 		//连接状态
			Show_Str(30,100,200,12,"发送数据:",12,0); 	//发送数据
			Show_Str(30,115,200,12,"接收数据:",12,0);	//接收数据
			atk_8266_wificonf_show(30,180,"请设置路由器无线参数为:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
			POINT_COLOR=BLUE;
			Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//连接状态
			USART3_RX_STA=0;
//			while(1)
//			{
						atk_8266_quit_trans();
						atk_8266_send_cmd("AT+CIPSEND","OK",20);         //开始透传           
						sprintf((char*)p,"ATK-8266%s测试%d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
						Show_Str(30+54,100,200,12,p,12,0);
						u3_printf("%s",p);
						timex=100;
			
				if(timex)timex--;
				if(timex==1) {
					LCD_Fill(30+54,100,239,112,WHITE);
					printf("%d",netpro);
					printf("为netpro状态\r\n");
				}
				t++;
				delay_ms(10);
				if(USART3_RX_STA&0X8000)		//接收到一次数据了
				{ 
					rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
					USART3_RX_BUF[rlen]=0;		//添加结束符 
					printf("%s",USART3_RX_BUF);	//发送到串口   
					sprintf((char*)p,"收到%d字节,内容如下",rlen);//接收到的字节数 
					LCD_Fill(30+54,115,239,130,WHITE);
					POINT_COLOR=BRED;
					Show_Str(30+54,115,156,12,p,12,0); 			//显示接收到的数据长度
					POINT_COLOR=BLUE;
					LCD_Fill(30,130,239,319,WHITE);
					Show_Str(30,130,180,190,USART3_RX_BUF,12,0);//显示接收到的数据  
					USART3_RX_STA=0;
					if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
					else t=0;                   //状态为已经连接了,10秒后再检查
				}  
				if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
				{
//			//		LCD_Fill(30+54,125,239,130,WHITE);
//					LCD_Fill(60,80,120,92,WHITE);
					constate=atk_8266_consta_check();//得到连接状态
					if(constate=='+')Show_Str(30+30,80,200,12,"连接成功",12,0);  //连接状态
					else Show_Str(30+30,80,200,12,"连接失败",12,0); 	 
					t=0;
				}
				if((t%20)==0)LED0_Toggle;
				atk_8266_at_response(1);
//			}
	myfree(SRAMIN,p);		//释放内存 
	return res;		
}

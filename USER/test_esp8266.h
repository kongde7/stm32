#include "sys.h"

//用户配置参数
extern const u8* portnum;			//连接端口

extern const u8* wifista_ssid;		//WIFI STA SSID
extern const u8* wifista_encryption;//WIFI STA 加密方式
extern const u8* wifista_password; 	//WIFI STA 密码

void test_esp8266(void);
u8 test_sta(void);

#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "main.h"

//FreeRtos中使用
#include "FreeRTOS.h"
#include "task.h"

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

//WIFI和密码·
#define ESP8266_WIFI_INFO		"AT+CWJAP=\"Moning\",\"12345678\"\r\n"
//获取时间API       
#define Time_TCP		"AT+CIPSTART=\"TCP\",\"api.k780.com\",80\r\n"
//获取时间GET报文
#define Time_GET		"GET http://api.k780.com/?app=life.time&appkey=76632&sign=ca56f969fece0fef96e2567442691a1a&format=json\r\n"        //填写独属于你的api

void ESP8266_Clear(void);
_Bool ESP8266_WaitRecive(void);
_Bool ESP8266_SendCmd(char *cmd, char *res);
void ESP8266_SendData(unsigned char *data, unsigned short len);

void ESP8266_Init(void);		

// 新增函数声明
void ESP8266_UART_Receive_Init(void);

#endif

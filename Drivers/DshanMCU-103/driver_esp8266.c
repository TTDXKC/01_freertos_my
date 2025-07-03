#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"

//FreeRtos中使用
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "usart.h"

#include "driver_esp8266.h"

extern UART_HandleTypeDef huart2;

//ESP8266数据存放
uint8_t esp8266_buf[300] = {0};
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;

// 定义用于接收单个字节的缓冲区
uint8_t USART_RX_BUF;

// 初始化ESP8266接收中断
void ESP8266_UART_Receive_Init(void)
{
    // 开启接收中断
    HAL_UART_Receive_IT(&huart2, &USART_RX_BUF, 1);
}

// UART接收中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2)
    {
        // 将接收到的数据存入缓冲区
        if(esp8266_cnt < sizeof(esp8266_buf) - 1)
        {
            esp8266_buf[esp8266_cnt++] = USART_RX_BUF;
        }
        
        // 重新启动接收中断，接收下一个字节
        HAL_UART_Receive_IT(&huart2, &USART_RX_BUF, 1);
    }
}

/**************************************************************************/
//函数作用：ESP8266_Init初始化函数
//函数名称：ESP8266_Init(void);
//内部参数：
//修改日期：2022年4月18日  下午16：18
/**************************************************************************/
void ESP8266_Init(void)
{
    // 初始化接收中断
    ESP8266_UART_Receive_Init();
//    OLED_PrintString(0,0,"Receive_Init!");
    ESP8266_Clear();//清除缓冲

    /*让WIFI退出透传模式 要发两次*/
//    while(ESP8266_SendCmd("+++", ""));//这是一个死循环，目的结束透传模式
//    OLED_PrintString(0,2,"+++");

    /*让WIFI清除Flah*/
    //printf("0.恢复出厂设置成功\r\n");

//    while(ESP8266_SendCmd("AT+RESTORE\r\n", "ready"));//恢复出厂设置

    
    while(ESP8266_SendCmd("AT\r\n", "OK"));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //加一步ESP8266复位操作
    //printf("2.RST复位成功\r\n");
    ESP8266_SendCmd("AT+RST\r\n", "");
    vTaskDelay(500);
    ESP8266_SendCmd("AT+CIPCLOSE\r\n", "");//断开与服务器的连接
    vTaskDelay(500);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //printf("3.CWMODE设置工作模式，保存到Flash\r\n");

    while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"));//没有CUR就是保存到Flash，AT+CWMODE_CUR设置模块工作模式为station，不保存到Flash
    //printf("4.AT+CIPMUX单连接模式设置成功\r\n");

    while(ESP8266_SendCmd("AT+CIPMUX=0\r\n", "OK"));//AT+CIPMUX=0 设置为单连接模式
    //printf("5.寻找对应的WIFI名称和密码\r\n");

    while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "WIFI GOT IP"));
    //printf("6.ESP8266_Init连接WIFI成功\r\n");
}




/**下面的代码来源于：
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	esp8266.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-08
	*
	*	版本： 		V1.0
	*
	*	说明： 		ESP8266的简单驱动
	*
	*	修改记录：
	************************************************************
	************************************************************
	************************************************************
**/
//==========================================================
//	函数名称：	ESP8266_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：
//==========================================================
void ESP8266_Clear(void)
{

    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;

}
//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{
    char cmdBuf[32];

    ESP8266_Clear();								//清空接收缓存
    sprintf(cmdBuf, "AT+CIPSEND\r\n");		//发送命令

    if(!ESP8266_SendCmd(cmdBuf, ">"))				//收到'>'时可以发送数据
    {
        //printf("8.开始处于透传发送状态!\r\n");
        /*发送请求数据*/
        Usart_SendString(&huart2, data, len);		//发送设备连接请求数据
    }
}



//==========================================================
//	函数名称：	ESP8266_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

    if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
        return REV_WAIT;

    if(esp8266_cnt == esp8266_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
    {
        esp8266_cnt = 0;							//清0接收计数

        return REV_OK;								//返回接收完成标志
    }

    esp8266_cntPre = esp8266_cnt;					//置为相同

    return REV_WAIT;								//返回接收未完成标志

}


//==========================================================
//	函数名称：	ESP8266_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
   {
       uint16_t timeOut = 300;
       char debug_buf[16] = {0};
       
       Usart_SendString(&huart2, (unsigned char *)cmd, strlen((const char *)cmd));
       
       while(timeOut--)
       {
           if(ESP8266_WaitRecive() == REV_OK)
           { 
               if(strstr((const char *)esp8266_buf, res) != NULL)
               {
                   ESP8266_Clear();
                   return 0;
               }
           }
           vTaskDelay(100);
       }
       
       return 1;
   }


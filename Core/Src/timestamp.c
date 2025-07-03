#include "timestamp.h"
#include "oled.h"

int c_stamptime = 0;
extern uint8_t esp8266_buf[300];




Times stamp_to_standard(int stampTime)
{
	time_t tick = (time_t)stampTime;
	struct tm tm;
	char s[100];
	Times standard;
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);

	standard.Year = atoi(s);
	standard.Mon = atoi(s+5);
	standard.Day = atoi(s+8);
	standard.Hour = atoi(s+11);
	standard.Min = atoi(s+14);
	standard.Second = atoi(s+17);
	
	return standard;
}

/**
  * @brief  从JSON字符串中提取时间戳值
  * @param  json_str: 要解析的JSON字符串
  * @retval 提取的时间戳值，解析失败则返回0
  */
int extract_timestamp(const char *json_str)
{
    const char *timestamp_key = "\"timestamp\":\"";
    char *timestamp_pos = strstr(json_str, timestamp_key);
    
    // 如果找到时间戳字段
    if (timestamp_pos != NULL)
    {
        // 指针移动到值的开始位置
        timestamp_pos += strlen(timestamp_key);
        
        // 提取数字字符串
        char timestamp_str[20] = {0};
        int i = 0;
        
        // 复制数字直到遇到引号
        while (timestamp_pos[i] != '\"' && i < 19)
        {
            timestamp_str[i] = timestamp_pos[i];
            i++;
        }
        
        // 转换为整数并返回
        return atoi(timestamp_str);
    }
    
    return 0; // 解析失败返回0
}

/**
  * @brief  处理ESP8266接收到的JSON时间数据
  * @param  无
  * @retval 时间戳值
  */
int process_time_json(void)
{
    int timestamp = 0;
    uint8_t json_buffer[300] = {0};
    
    // 从ESP8266获取数据
    ESP8266_GetIPD_GET(100, json_buffer);
    
    // 提取时间戳
    timestamp = extract_timestamp((const char*)json_buffer);
    
    // 如果成功获取时间戳，可以存储到全局变量
    if (timestamp > 0)
    {
        c_stamptime = timestamp;
    }
    
    return timestamp;
}


//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	copy天气数据到buff数组里面
//
//	返回参数：	平台返回的原始数据
//
//	说明：		copy天气数据到buff
//==========================================================
unsigned char *ESP8266_GetIPD_GET(unsigned short timeOut, uint8_t *buff) //这里我用了一个全局变量将esp8266buf储存到这个全局变量里面
{

    vTaskDelay(timeOut);

    strcpy((char*)buff, (char*)esp8266_buf);
    return buff;
}


/**************************************************************************/
//函数作用：获取时间
//函数名称：Get_current_time();
//内部参数：
//返回参数：int 时间戳
/**************************************************************************/
int Get_current_time(void)
{
    int timestamp = 0;
    
    ESP8266_Init();	     //连接Wifi的ESP8266初始化
    ESP8266_Clear();

    while(ESP8266_SendCmd(Time_TCP, "CONNECT"));
    
    //printf("6.访问服务器成功 OK\r\n");

    while(ESP8266_SendCmd("AT+CIPMODE=1\r\n", "OK"));//开启透传模式

    //printf("7.开启透传模式成功 OK\r\n");

    /*sizeof(Time_GET)，必须用sizeof函数，用strlen没有用*/
    ESP8266_SendData((uint8_t *)Time_GET, sizeof(Time_GET)); //发送AT+CIPSEND  以及 Time_GET
    // 等待接收数据
    vTaskDelay(1000); // 给服务器响应足够的时间
    
    // 处理接收到的JSON数据并提取时间戳
    timestamp = process_time_json();
    OLED_ShowString(0,32,"sueccess",OLED_8X16_HALF);
    OLED_Update();
    
    // 退出透传模式前不要清除缓存，确保数据已处理
    while(ESP8266_SendCmd("+++", ""));      /*退出透传模式，发送两次*/
    //printf("9.退出透传模式成功 OK\r\n");
    
    return timestamp; // 返回提取的时间戳
}

int timestamp_updata(void)
{
    int timestamp = 0;
    ESP8266_SendData((uint8_t *)Time_GET, sizeof(Time_GET));
    vTaskDelay(1000);
    timestamp = process_time_json();
    while(ESP8266_SendCmd("+++", ""));
    return timestamp;
}

void save_time_init(RTC_TimeTypeDef *RTC_TimeStruct, RTC_DateTypeDef *RTC_DataStruct, Times time_now) {
    // 调整为北京时间 (UTC+8)
    uint8_t hour_beijing = (uint8_t)(time_now.Hour + 8);
    
    // 处理跨天情况
    if (hour_beijing >= 24) {
        hour_beijing -= 24;
        // 这里如果需要处理日期，还需要增加日期处理逻辑
    }
    
    RTC_TimeStruct->Hours = hour_beijing;
    RTC_TimeStruct->Minutes = (uint8_t)time_now.Min;
    RTC_TimeStruct->Seconds = (uint8_t)(time_now.Second+2);
    RTC_DataStruct->Date = (uint8_t)time_now.Day;
    RTC_DataStruct->Month = (uint8_t)time_now.Mon;
    RTC_DataStruct->Year = (uint8_t)25;
}

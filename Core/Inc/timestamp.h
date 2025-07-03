#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__ 

#include "driver_esp8266.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct times
{
	int Year;
	int Mon;
	int Day;
	int Hour;
	int Min;
	int Second;
}Times;
//函数声明
Times stamp_to_standard(int stampTime);

int extract_timestamp(const char *json_str);

int process_time_json(void);

unsigned char *ESP8266_GetIPD_GET(unsigned short timeOut,uint8_t *buff);
int Get_current_time(void);
int timestamp_updata(void);

void save_time_init(RTC_TimeTypeDef *RTC_TimeStruct,RTC_DateTypeDef *RTC_DataStruct,Times time_now);

#endif

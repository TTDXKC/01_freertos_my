#ifndef __BSP_HW269_H
#define __BSP_HW269_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/**
  * @brief  初始化PWM输出
  * @param  无
  * @retval 无
  */
void PWM_Init(void);

/**
  * @brief  设置PWM占空比
  * @param  compare: PWM比较值，范围0-100
  * @retval 无
  */
void PWM_SetCompare(uint16_t compare);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_HW269_H */ 

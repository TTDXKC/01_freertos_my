#include "bsp_hw269.h"

TIM_HandleTypeDef htim3;

/**
  * @brief  初始化PWM输出
  * @param  无
  * @retval 无
  */
void PWM_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* 使能定时器和GPIO时钟 */
  __HAL_RCC_TIM3_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* 配置GPIO为复用推挽输出 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* 初始化TIM3 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 72-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 100-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_Base_Init(&htim3);
  
  /* 配置时钟源 */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);
  
  /* 配置PWM */
  HAL_TIM_PWM_Init(&htim3);
  
  /* 配置主模式 */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);
  
  /* 配置PWM通道3 */
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 50;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
  
  /* 启动PWM输出 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

/**
  * @brief  设置PWM占空比
  * @param  compare: PWM比较值，范围0-100
  * @retval 无
  */
void PWM_SetCompare(uint16_t compare)
{
  /* 限制占空比在0-100之间 */
  if (compare > 100)
  {
    compare = 100;
  }
  
  /* 设置PWM比较值 */
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, compare);
}

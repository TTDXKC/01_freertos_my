#include "key.h"

void key_func(void){
    HAL_GPIO_ReadPin(KEY1_Port, KEY1_Pin);
    HAL_GPIO_ReadPin(KEY2_Port, KEY2_Pin);
}
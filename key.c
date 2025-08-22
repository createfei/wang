#include "key.h"
#include "config.h"

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
}

// 简单按键扫描，返回1~4，无按键返回0
uint8_t Key_Scan(void)
{
    if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY1_PIN) == 0) return 1;
    if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY2_PIN) == 0) return 2;
    if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY3_PIN) == 0) return 3;
    if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY4_PIN) == 0) return 4;
    return 0;
}
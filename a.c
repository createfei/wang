#include "stm32f10x.h"

// 假设我们有4个按键，分别连接到PA0~PA3
#define KEY1_PIN GPIO_Pin_0
#define KEY2_PIN GPIO_Pin_1
#define KEY3_PIN GPIO_Pin_2
#define KEY4_PIN GPIO_Pin_3

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置PA0~PA3为输入上拉
    GPIO_InitStructure.GPIO_Pin = KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void USART_Config(void)
{
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 配置USART1 Tx (PA9)为复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置USART1 Rx (PA10)为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600; // HC-05 默认波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

// 发送一个字符
void USART_SendChar(USART_TypeDef* USARTx, uint8_t ch)
{
    USART_SendData(USARTx, ch);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

// 按键扫描，返回按键编号（1~4），无按键返回0
uint8_t Key_Scan(void)
{
    if (GPIO_ReadInputDataBit(GPIOA, KEY1_PIN) == 0) return 1;
    if (GPIO_ReadInputDataBit(GPIOA, KEY2_PIN) == 0) return 2;
    if (GPIO_ReadInputDataBit(GPIOA, KEY3_PIN) == 0) return 3;
    if (GPIO_ReadInputDataBit(GPIOA, KEY4_PIN) == 0) return 4;
    return 0;
}

int main(void)
{
    GPIO_Config();
    USART_Config();

    uint8_t last_key = 0;
    while (1)
    {
        uint8_t key = Key_Scan();
        if (key != 0 && key != last_key)
        {
            // 按下新按键，发送编号
            USART_SendChar(USART1, key + '0'); // 发送'1'~'4'
            last_key = key;
        }
        if (key == 0) last_key = 0; // 松开按键后允许再次触发
    }
}
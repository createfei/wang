#include "stm32f10x.h"
#include "key.h"
#include "usart.h"

int main(void)
{
    Key_Init();
    USART_BT_Init();

    uint8_t last_key = 0;
    while(1)
    {
        uint8_t key = Key_Scan();
        if(key != 0 && key != last_key)
        {
            USART_BT_SendChar(key + '0');
            last_key = key;
        }
        if(key == 0) last_key = 0;
    }
}
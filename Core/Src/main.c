#include "main.h"
#include "keypad.h"
#include "keypad_config.h"
#include "keypad_queue.h"
#include <stdio.h>

I2C_HandleTypeDef hi2c1;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim2;     
UART_HandleTypeDef huart2;   

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);


int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 100);
    return ch;
}

int main(void)
{

    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();         
    MX_TIM2_Init();          
    MX_USART2_UART_Init();   

    Keypad_Init();        

    printf("\r\n===== KEYPAD MODULE 1 =====\r\n");
    printf("San sang nhan phim...\r\n");

    while (1)
    {
        KeyEvent ev;
        while (Keypad_PollEvent(&ev))
        {
            const char *ev_name[] = {"NONE", "PRESSED", "HOLD", "RELEASED"};
            printf("[KEY] '%c' %s @ %lu ms\r\n",
                   ev.key,
                   ev_name[ev.event],
                   (unsigned long)HAL_GetTick());
        }

    }
}
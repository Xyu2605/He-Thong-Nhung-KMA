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
void Error_Handler(void);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);



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
    MX_I2C1_Init();
    MX_SPI1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_USART2_UART_Init();

    /* --- Init Module 3 (Servo + Buzzer + Password) --- */
    Password_Init();
    Servo_Init();
    Buzzer_Init();

    /* --- Init Module 4 (RFID) --- */
    RFID_Init();

    /* --- Init Module 1 (Keypad) --- */
    Keypad_Init();

    /* --- Init Module 2 (FSM + LCD) — cuối cùng --- */
    AppFSM_Init();

    while (1)
    {
        /* 1. Lấy event keypad → đẩy vào FSM */
        KeyEvent ev;
        while (Keypad_PollEvent(&ev)) {
            if (ev.event == KEY_EVENT_PRESSED) {
                printf("[KEY] '%c'\r\n", ev.key);
                AppFSM_HandleKey(ev.key);
            }
        }

        /* 2. RFID — chỉ hoạt động khi ở màn login */
        if (AppFSM_GetState() == STATE_LOGIN) {
            if (RFID_PollAdmin()) {
                printf("[RFID] The admin!\r\n");
                AppFSM_HandleRFID();
            }
        }

        /* 3. Update FSM — xử lý timer, buzzer, đóng cửa */
        AppFSM_Update();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        Keypad_Scan_Tick();    
    }

}


void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) Error_Handler();
}


static void MX_GPIO_Init(void)      
static void MX_I2C1_Init(void)       
static void MX_TIM2_Init(void)        
static void MX_TIM3_Init(void)        
static void MX_USART2_UART_Init(void) 

static void MX_SPI1_Init(void)
{
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) Error_Handler();
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) { }
}
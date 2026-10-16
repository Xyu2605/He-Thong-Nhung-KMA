#include "keypad_timer.h"
#include "keypad.h"
#include "main.h"

/* Hàm này được HAL gọi tự động mỗi khi TIM2 tràn (10ms) */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        Keypad_Scan_Tick();   // Quét ma trận 1 lần
    }
}
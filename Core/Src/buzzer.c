#include "buzzer.h"
#include "app_config.h"

static bool     buzzer_active = false;
static uint32_t buzzer_start_tick = 0;

void Buzzer_On(void)
{
    HAL_GPIO_WritePin(
        Buzzer_GPIO_Port,
        Buzzer_Pin,
        GPIO_PIN_SET
    );

    buzzer_active = true;
    buzzer_start_tick = HAL_GetTick();
}

void Buzzer_Off(void)
{
    HAL_GPIO_WritePin(
        Buzzer_GPIO_Port,
        Buzzer_Pin,
        GPIO_PIN_RESET
    );

    buzzer_active = false;
}

bool Buzzer_IsActive(void)
{
    return buzzer_active;
}


void Buzzer_Update(void)
{
    if (buzzer_active)
    {
        if (HAL_GetTick() - buzzer_start_tick >= BUZZER_TIME_MS)
        {
            Buzzer_Off();
        }
    }
}
#include "servo.h"
#include "app_config.h"

extern TIM_HandleTypeDef htim3;

/* COPY NGUYÊN từ main.c cũ */
void Servo_SetAngle(uint8_t angle)
{
    if (angle > 180)
        angle = 180;

    uint32_t ccr =
        SERVO_CCR_0_DEG +
        ((uint32_t)angle *
        (SERVO_CCR_180_DEG - SERVO_CCR_0_DEG))
        / 180;

    __HAL_TIM_SET_COMPARE(
        &htim3,
        TIM_CHANNEL_4,
        ccr
    );
}

void Door_Open(void)
{
    Servo_SetAngle(90);
}

void Door_Close(void)
{
    Servo_SetAngle(0);
}
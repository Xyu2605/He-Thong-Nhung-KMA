#include "keypad.h"
#include "main.h"

char Keypad_Scan(void)
{
    const char keymap[4][4] =
    {
        {'*','0','#','A'},
        {'7','8','9','B'},
        {'4','5','6','C'},
        {'1','2','3','D'}
    };

    GPIO_TypeDef *colPort[4] =
    {
        GPIOA,
        GPIOB,
        GPIOB,
        GPIOB
    };
  uint16_t colPin[4] =
    {
        GPIO_PIN_8,
        GPIO_PIN_15,
        GPIO_PIN_14,
        GPIO_PIN_13
    };

    GPIO_TypeDef *rowPort[4] =
    {
        GPIOA,
        GPIOA,
        GPIOA,
        GPIOA
    };

    uint16_t rowPin[4] =
    {
        GPIO_PIN_12,
        GPIO_PIN_11,
        GPIO_PIN_10,
        GPIO_PIN_9
    };

    for (int c = 0; c < 4; c++)
    {
        HAL_GPIO_WritePin(
            GPIOA,
            GPIO_PIN_8,
            GPIO_PIN_SET
        );

        HAL_GPIO_WritePin(
            GPIOB,
            GPIO_PIN_13 |
            GPIO_PIN_14 |
            GPIO_PIN_15,
            GPIO_PIN_SET
        );

        HAL_GPIO_WritePin(
            colPort[c],
            colPin[c],
            GPIO_PIN_RESET
        );

        for (volatile int i = 0; i < 200; i++);

        for (int r = 0; r < 4; r++)
        {
            if (HAL_GPIO_ReadPin(
                    rowPort[r],
                    rowPin[r])
                == GPIO_PIN_RESET)
            {
                return keymap[r][c];
            }
        }
    }

    return 0;
}

#include "app_ui.h"
#include "app_fsm.h"
#include "app_config.h"

void LCD_ShowInput(const char *title)
{
    char hidden[17] = {0};

    /* Tạo chuỗi **** */
    for (uint8_t i = 0; i < input_len && i < 16; i++)
    {
        hidden[i] = '*';
    }

    hidden[input_len] = '\0';

    /* Xóa LCD */
    CLCD_I2C_Clear(&LCD1);

    /* Dòng 1 */
    CLCD_I2C_SetCursor(&LCD1, 0, 0);
    CLCD_I2C_WriteString(&LCD1, (char *)title);

    /* Dòng 2 */
    CLCD_I2C_SetCursor(&LCD1, 0, 1);

    if (input_len == 0)
    {
        CLCD_I2C_WriteString(&LCD1, "_");
    }
    else
    {
        CLCD_I2C_WriteString(&LCD1, hidden);
    }
}
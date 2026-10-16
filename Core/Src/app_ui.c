#include "app_ui.h"
#include "CLCD_I2C.h"
#include "password.h"
#include <string.h>
#include <stdio.h>

/* Handle LCD (khai báo toàn cục) */
CLCD_I2C_Name LCD1;

/* Địa chỉ I2C của PCF8574 (thường 0x4E cho module 16x2) */
#define LCD_I2C_ADDR     0x4E
#define LCD_COLS         16
#define LCD_ROWS         2

/* ============================================================================
 * PRIVATE
 * ==========================================================================*/

/* Vẽ chuỗi dấu * theo độ dài input */
static void AppUI_DrawMasked(void)
{
    uint8_t len = Password_GetInputLen();
    char masked[LCD_COLS + 1];
    uint8_t i;

    if (len == 0) {
        strcpy(masked, "_");
    } else {
        for (i = 0; i < len && i < LCD_COLS; i++) {
            masked[i] = '*';
        }
        masked[i] = '\0';
    }

    CLCD_I2C_SetCursor(&LCD1, 0, 1);
    CLCD_I2C_WriteString(&LCD1, masked);
    /* Đệm khoảng trắng để xóa ký tự cũ */
    for (i = strlen(masked); i < LCD_COLS; i++) {
        CLCD_I2C_WriteChar(&LCD1, ' ');
    }
}

/* ============================================================================
 * PUBLIC
 * ==========================================================================*/

void AppUI_Init(void)
{
    CLCD_I2C_Init(&LCD1, &hi2c1, LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);
    CLCD_I2C_Clear(&LCD1);
}

void AppUI_ShowLogin(void)
{
    CLCD_I2C_Clear(&LCD1);
    CLCD_I2C_SetCursor(&LCD1, 0, 0);
    CLCD_I2C_WriteString(&LCD1, "Nhap mat khau");
    CLCD_I2C_SetCursor(&LCD1, 0, 1);
    CLCD_I2C_WriteString(&LCD1, "_");
}

void AppUI_ShowInputPassword(void)
{
    CLCD_I2C_SetCursor(&LCD1, 0, 0);
    CLCD_I2C_WriteString(&LCD1, "Nhap mat khau   ");
    AppUI_DrawMasked();
}

void AppUI_ShowWrongPassword(uint8_t attempts_left)
{
    char buf[LCD_COLS + 1];

    CLCD_I2C_Clear(&LCD1);
    CLCD_I2C_SetCursor(&LCD1, 0, 0);
    CLCD_I2C_WriteString(&LCD1, "SAI MAT KHAU");

    snprintf(buf, sizeof(buf), "Con %d lan", attempts_left);
    CLCD_I2C_SetCursor(&LCD1, 0, 1);
    CLCD_I2C_WriteString(&LCD1, buf);
}

void AppUI_ShowDoorOpen(void)
{
    CLCD_I2C_Clear(&LCD1);
    CLCD_I2C_SetCursor(&LCD1, 0, 0);
    CLCD_I2C_WriteString(&LCD1, "MAT KHAU DUNG");
    CLCD_I2C_SetCursor(&LCD1, 0, 1);
    CLCD_I2C_WriteString(&LCD1, "CUA DA MO");
}

void AppUI_ShowLocked(uint32_t seconds_left)
{
    char buf[LCD_COLS + 1];

    CLCD_I2C_Clear(&LCD1);
    CLCD_I2C_SetCursor(&LCD1, 0, 0);
    CLCD_I2C_WriteString(&LCD1, "HE THONG KHOA");

    snprintf(buf, sizeof(buf), "Con %lu giay", (unsigned long)seconds_left);
    CLCD_I2C_SetCursor(&LCD1, 0, 1);
    CLCD_I2C_WriteString(&LCD1, buf);
}

void AppUI_ShowAdminOldPw(void)
{
    CLCD_I2C_Clear(&LCD1);
    CLCD_I2C_SetCursor(&LCD1, 0, 0);
    CLCD_I2C_WriteString(&LCD1, "MK Admin cu:");
    CLCD_I2C_SetCursor(&LCD1, 0, 1);
    CLCD_I2C_WriteString(&LCD1, "_");
}

void AppUI_ShowAdminNewPw(void)
{
    CLCD_I2C_Clear(&LCD1);
    CLCD_I2C_SetCursor(&LCD1, 0, 0);
    CLCD_I2C_WriteString(&LCD1, "Nhap MK moi:");
    CLCD_I2C_SetCursor(&LCD1, 0, 1);
    CLCD_I2C_WriteString(&LCD1, "_");
}

void AppUI_ShowChangeSuccess(void)
{
    CLCD_I2C_Clear(&LCD1);
    CLCD_I2C_SetCursor(&LCD1, 0, 0);
    CLCD_I2C_WriteString(&LCD1, "DOI MAT KHAU");
    CLCD_I2C_SetCursor(&LCD1, 0, 1);
    CLCD_I2C_WriteString(&LCD1, "THANH CONG");
}

void AppUI_ShowAdminRejected(void)
{
    CLCD_I2C_Clear(&LCD1);
    CLCD_I2C_SetCursor(&LCD1, 0, 0);
    CLCD_I2C_WriteString(&LCD1, "SAI MK ADMIN");
    CLCD_I2C_SetCursor(&LCD1, 0, 1);
    CLCD_I2C_WriteString(&LCD1, "Tu choi truy cap");
}
#include "app_fsm.h"
#include "app_ui.h"
#include "app_config.h"
#include "password.h"      /* system_password extern */
#include "servo.h"
#include "buzzer.h"
#include <string.h>
#include <stdio.h>

AppState app_state = ST_LOGIN;
char     input_buffer[17] = {0};
uint8_t  input_len = 0;


static uint8_t  wrong_count = 0;
static uint32_t door_open_tick = 0;
static uint32_t lock_start_tick = 0;
static uint32_t last_display_second = 0;


void ClearInput(void)
{
    memset(input_buffer, 0, sizeof(input_buffer));
    input_len = 0;
}

void AppendInput(char key)
{
    if (input_len < PASSWORD_MAX_LEN)
    {
        input_buffer[input_len++] = key;
        input_buffer[input_len] = '\0';
    }
}

void BackspaceInput(void)
{
    if (input_len > 0)
    {
        input_len--;
        input_buffer[input_len] = '\0';
    }
}


void HandleKey(char key)
{
    switch (app_state)
    {
        case ST_LOGIN:

            if (key >= '0' && key <= '9')
            {
                AppendInput(key);
                LCD_ShowInput("Nhap mat khau");
            }
            else if (key == '*')
            {
                BackspaceInput();
                LCD_ShowInput("Nhap mat khau");
            }
            else if (key == '#')
            {
                /* ===== MẬT KHẨU ĐÚNG ===== */
                if (strcmp(input_buffer, system_password) == 0)
                {
                    wrong_count = 0;
                    ClearInput();

                    CLCD_I2C_Clear(&LCD1);

                    CLCD_I2C_SetCursor(&LCD1, 0, 0);
                    CLCD_I2C_WriteString(&LCD1, "MAT KHAU DUNG");

                    CLCD_I2C_SetCursor(&LCD1, 0, 1);
                    CLCD_I2C_WriteString(&LCD1, "CUA DA MO");

                    Door_Open();

                    door_open_tick = HAL_GetTick();
                    app_state = ST_DOOR_OPEN;
                }
                /* ===== MẬT KHẨU SAI ===== */
                else
                {
                    wrong_count++;
                    ClearInput();

                    if (wrong_count >= 3)
                    {
                        wrong_count = 0;

                        Door_Close();
                        Buzzer_On();

                        lock_start_tick = HAL_GetTick();
                        last_display_second = 0;

                        app_state = ST_LOCKED;

                        CLCD_I2C_Clear(&LCD1);
                        CLCD_I2C_SetCursor(&LCD1, 0, 0);
                        CLCD_I2C_WriteString(&LCD1, "HE THONG KHOA");
                        CLCD_I2C_SetCursor(&LCD1, 0, 1);
                        CLCD_I2C_WriteString(&LCD1, "Con 30 giay");
                    }
                    else
                    {
                        char msg[17];
                        snprintf(msg, sizeof(msg), "Con %d lan", 3 - wrong_count);

                        CLCD_I2C_Clear(&LCD1);
                        CLCD_I2C_SetCursor(&LCD1, 0, 0);
                        CLCD_I2C_WriteString(&LCD1, "SAI MAT KHAU");
                        CLCD_I2C_SetCursor(&LCD1, 0, 1);
                        CLCD_I2C_WriteString(&LCD1, msg);

                        HAL_Delay(1000);
                        LCD_ShowInput("Nhap mat khau");
                    }
                }
            }
            break;

        /* ===== ADMIN — MK HIỆN TẠI ===== */
        case ST_ADMIN_OLD_PASSWORD:

            if (key >= '0' && key <= '9')
            {
                AppendInput(key);
                LCD_ShowInput("MK hien tai:");
            }
            else if (key == '*')
            {
                BackspaceInput();
                LCD_ShowInput("MK hien tai:");
            }
            else if (key == '#')
            {
                if (strcmp(input_buffer, system_password) == 0)
                {
                    ClearInput();
                    app_state = ST_ADMIN_NEW_PASSWORD;
                    LCD_ShowInput("Nhap MK moi:");
                }
                else
                {
                    ClearInput();
                    app_state = ST_LOGIN;

                    CLCD_I2C_Clear(&LCD1);
                    CLCD_I2C_WriteString(&LCD1, "SAI MAT KHAU");

                    HAL_Delay(1000);
                    LCD_ShowInput("Nhap mat khau");
                }
            }
            break;

        /* ===== ADMIN — MK MỚI ===== */
        case ST_ADMIN_NEW_PASSWORD:

            if (key >= '0' && key <= '9')
            {
                AppendInput(key);
                LCD_ShowInput("Nhap MK moi:");
            }
            else if (key == '*')
            {
                BackspaceInput();
                LCD_ShowInput("Nhap MK moi:");
            }
            else if (key == '#')
            {
                if (input_len >= 4)
                {
                    strcpy(system_password, input_buffer);
                    Password_SaveFlash();

                    ClearInput();
                    app_state = ST_LOGIN;

                    CLCD_I2C_Clear(&LCD1);
                    CLCD_I2C_SetCursor(&LCD1, 0, 0);
                    CLCD_I2C_WriteString(&LCD1, "DOI MAT KHAU");
                    CLCD_I2C_SetCursor(&LCD1, 0, 1);
                    CLCD_I2C_WriteString(&LCD1, "THANH CONG");

                    HAL_Delay(1500);
                    LCD_ShowInput("Nhap mat khau");
                }
            }
            break;

        case ST_DOOR_OPEN:
        case ST_LOCKED:
        default:
            break;
    }
}

void CheckSystemTimers(void)
{
    uint32_t now = HAL_GetTick();

    /* =====================================
       CỬA TỰ ĐỘNG ĐÓNG — 5 giây
       ===================================== */
    if (app_state == ST_DOOR_OPEN)
    {
        if (now - door_open_tick >= DOOR_OPEN_TIME_MS)
        {
            Door_Close();
            ClearInput();
            app_state = ST_LOGIN;
            LCD_ShowInput("Nhap mat khau");
        }
    }

    /* =====================================
       KHÓA HỆ THỐNG 30 GIÂY — ĐẾM NGƯỢC
       ===================================== */
    if (app_state == ST_LOCKED)
    {
        uint32_t elapsed = now - lock_start_tick;

        /* Hiển thị đếm ngược mỗi 1 giây */
        if (now - last_display_second >= 1000)
        {
            last_display_second = now;

            uint32_t remaining;
            if (elapsed < LOCK_TIME_MS)
                remaining = (LOCK_TIME_MS - elapsed + 999) / 1000;
            else
                remaining = 0;

            char buf[17];
            snprintf(buf, sizeof(buf), "Con %lu giay", (unsigned long)remaining);

            CLCD_I2C_Clear(&LCD1);
            CLCD_I2C_SetCursor(&LCD1, 0, 0);
            CLCD_I2C_WriteString(&LCD1, "HE THONG KHOA");
            CLCD_I2C_SetCursor(&LCD1, 0, 1);
            CLCD_I2C_WriteString(&LCD1, buf);
        }

        /* Hết 30 giây */
        if (elapsed >= LOCK_TIME_MS)
        {
            Buzzer_Off();
            ClearInput();
            app_state = ST_LOGIN;
            LCD_ShowInput("Nhap mat khau");
        }
    }

    Buzzer_Update();
}
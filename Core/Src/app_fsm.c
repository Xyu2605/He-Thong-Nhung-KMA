#include "app_fsm.h"
#include "app_ui.h"
#include "password.h"
#include "servo.h"
#include "buzzer.h"
#include "main.h"
#include <string.h>

/* ============================================================================
 * PRIVATE DATA
 * ==========================================================================*/

static AppState app_state = STATE_LOGIN;
static uint8_t  wrong_count = 0;

/* Timer */
static uint32_t door_open_tick = 0;
static uint32_t lock_start_tick = 0;
static uint32_t last_locked_display = 0;

/* Hằng số */
#define MAX_WRONG_ATTEMPTS    3U
#define DOOR_OPEN_TIME_MS     5000UL
#define LOCK_TIME_MS          30000UL

/* ============================================================================
 * PRIVATE HELPERS
 * ==========================================================================*/

static void AppFSM_EnterLogin(void)
{
    Password_ClearInput();
    app_state = STATE_LOGIN;
    AppUI_ShowLogin();
}

static void AppFSM_EnterLocked(void)
{
    wrong_count = 0;
    Door_Close();
    Buzzer_On();
    lock_start_tick = HAL_GetTick();
    last_locked_display = 0;
    app_state = STATE_LOCKED;
    AppUI_ShowLocked(LOCK_TIME_MS / 1000);
}

static void AppFSM_EnterDoorOpen(void)
{
    Password_ClearInput();
    door_open_tick = HAL_GetTick();
    app_state = STATE_DOOR_OPEN;
    Door_Open();
    AppUI_ShowDoorOpen();
}

/* ============================================================================
 * PUBLIC FUNCTIONS
 * ==========================================================================*/

void AppFSM_Init(void)
{
    AppUI_Init();
    wrong_count = 0;
    AppFSM_EnterLogin();
}

AppState AppFSM_GetState(void)
{
    return app_state;
}

void AppFSM_HandleKey(char key)
{
    switch (app_state)
    {
    /* ==========================================================
     * STATE_LOGIN — Nhập mật khẩu user
     * ========================================================== */
    case STATE_LOGIN:
        if (key >= '0' && key <= '9') {
            Password_Append(key);
            AppUI_ShowInputPassword();
        }
        else if (key == '*') {
            Password_Backspace();
            AppUI_ShowInputPassword();
        }
        else if (key == '#') {
            /* ===== Xác thực mật khẩu ===== */
            if (Password_VerifyUser(Password_GetInput())) {
                wrong_count = 0;
                AppFSM_EnterDoorOpen();
            } else {
                wrong_count++;

                if (wrong_count >= MAX_WRONG_ATTEMPTS) {
                    AppFSM_EnterLocked();
                } else {
                    uint8_t left = MAX_WRONG_ATTEMPTS - wrong_count;
                    Password_ClearInput();
                    AppUI_ShowWrongPassword(left);
                    HAL_Delay(1500);
                    AppFSM_EnterLogin();
                }
            }
        }
        break;

    /* ==========================================================
     * STATE_DOOR_OPEN — Không nhận phím
     * ========================================================== */
    case STATE_DOOR_OPEN:
        /* Bỏ qua */
        break;

    /* ==========================================================
     * STATE_LOCKED — Không nhận phím
     * ========================================================== */
    case STATE_LOCKED:
        /* Bỏ qua */
        break;

    /* ==========================================================
     * STATE_ADMIN_OLD_PW — Nhập MK admin hiện tại
     * ========================================================== */
    case STATE_ADMIN_OLD_PW:
        if (key >= '0' && key <= '9') {
            Password_Append(key);
            AppUI_ShowAdminOldPw();
        }
        else if (key == '*') {
            Password_Backspace();
            AppUI_ShowAdminOldPw();
        }
        else if (key == '#') {
            if (Password_VerifyAdmin(Password_GetInput())) {
                Password_ClearInput();
                app_state = STATE_ADMIN_NEW_PW;
                AppUI_ShowAdminNewPw();
            } else {
                Password_ClearInput();
                AppUI_ShowAdminRejected();
                HAL_Delay(1500);
                AppFSM_EnterLogin();
            }
        }
        break;

    /* ==========================================================
     * STATE_ADMIN_NEW_PW — Nhập MK mới
     * ========================================================== */
    case STATE_ADMIN_NEW_PW:
        if (key >= '0' && key <= '9') {
            Password_Append(key);
            AppUI_ShowAdminNewPw();
        }
        else if (key == '*') {
            Password_Backspace();
            AppUI_ShowAdminNewPw();
        }
        else if (key == '#') {
            if (Password_GetInputLen() >= 4) {
                Password_ChangeUser(Password_GetInput());
                Password_ClearInput();
                AppUI_ShowChangeSuccess();
                HAL_Delay(1500);
                AppFSM_EnterLogin();
            } else {
                /* Quá ngắn — yêu cầu nhập lại */
                AppUI_ShowAdminNewPw();
            }
        }
        break;

    default:
        break;
    }
}

void AppFSM_HandleRFID(void)
{
    /* Chỉ nhận RFID khi đang ở màn login */
    if (app_state == STATE_LOGIN) {
        Password_ClearInput();
        app_state = STATE_ADMIN_OLD_PW;
        AppUI_ShowAdminOldPw();
    }
}

void AppFSM_Update(void)
{
    uint32_t now = HAL_GetTick();

    /* ==========================================================
     * Tự đóng cửa sau DOOR_OPEN_TIME_MS
     * ========================================================== */
    if (app_state == STATE_DOOR_OPEN) {
        if (now - door_open_tick >= DOOR_OPEN_TIME_MS) {
            Door_Close();
            AppFSM_EnterLogin();
        }
    }

    /* ==========================================================
     * Đếm ngược lockout
     * ========================================================== */
    if (app_state == STATE_LOCKED) {
        uint32_t elapsed = now - lock_start_tick;

        if (now - last_locked_display >= 1000) {
            last_locked_display = now;

            uint32_t remaining_ms = (elapsed < LOCK_TIME_MS)
                                  ? (LOCK_TIME_MS - elapsed) : 0;
            uint32_t remaining_s  = (remaining_ms + 999) / 1000;

            AppUI_ShowLocked(remaining_s);
        }

        if (elapsed >= LOCK_TIME_MS) {
            Buzzer_Off();
            AppFSM_EnterLogin();
        }
    }

    /* ==========================================================
     * Buzzer auto-off
     * ========================================================== */
    Buzzer_Update();
}
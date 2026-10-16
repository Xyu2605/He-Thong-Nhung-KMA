#include "keypad.h"
#include "keypad_config.h"
#include "keypad_queue.h"

/* Keymap 2 chiều */
static const char keymap[4][4] = KEYPAD_MAP;

/* Con trỏ chân cột & hàng */
static GPIO_TypeDef *col_port[4] = {
    KP_C1_PORT, KP_C2_PORT, KP_C3_PORT, KP_C4_PORT
};
static const uint16_t col_pin[4] = {
    KP_C1_PIN, KP_C2_PIN, KP_C3_PIN, KP_C4_PIN
};
static GPIO_TypeDef *row_port[4] = {
    KP_R1_PORT, KP_R2_PORT, KP_R3_PORT, KP_R4_PORT
};
static const uint16_t row_pin[4] = {
    KP_R1_PIN, KP_R2_PIN, KP_R3_PIN, KP_R4_PIN
};

/* Ma trận trạng thái */
static uint8_t raw[4][4];
static uint8_t prev[4][4];
static uint8_t stable[4][4];
static uint16_t hold_count[4][4];

/* ==== Private: đọc 1 lần toàn ma trận ==== */
static void Keypad_ReadMatrix(uint8_t out[4][4])
{
    for (int c = 0; c < 4; c++) {
        /* Kéo tất cả cột lên HIGH */
        for (int i = 0; i < 4; i++) {
            HAL_GPIO_WritePin(col_port[i], col_pin[i], GPIO_PIN_SET);
        }
        /* Kéo cột c xuống LOW */
        HAL_GPIO_WritePin(col_port[c], col_pin[c], GPIO_PIN_RESET);

        /* Delay nhỏ để tín hiệu ổn định */
        for (volatile int i = 0; i < 50; i++);

        /* Đọc 4 hàng */
        for (int r = 0; r < 4; r++) {
            GPIO_PinState s = HAL_GPIO_ReadPin(row_port[r], row_pin[r]);
            out[r][c] = (s == GPIO_PIN_RESET) ? 1 : 0;
        }
    }

    /* Trả tất cả cột về HIGH */
    for (int i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(col_port[i], col_pin[i], GPIO_PIN_SET);
    }
}

/* ==== Public: gọi từ ngắt TIM2 mỗi 10ms ==== */
void Keypad_Scan_Tick(void)
{
    Keypad_ReadMatrix(raw);

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {

            /* Chỉ xử lý khi giá trị ổn định 2 chu kỳ */
            if (raw[r][c] != prev[r][c]) {
                prev[r][c] = raw[r][c];
                continue;   // chưa đủ 2 chu kỳ ổn định
            }

            /* Giá trị ổn định — so với stable */
            if (raw[r][c] != stable[r][c]) {
                stable[r][c] = raw[r][c];
                KeyEvent ev;
                ev.key = keymap[r][c];

                if (raw[r][c] == 1) {
                    ev.event = KEY_EVENT_PRESSED;
                    hold_count[r][c] = 0;
                } else {
                    ev.event = KEY_EVENT_RELEASED;
                    hold_count[r][c] = 0;
                }
                KeypadQueue_Push(ev);
            }
            /* Đang giữ phím */
            else if (stable[r][c] == 1) {
                hold_count[r][c]++;
                if (hold_count[r][c] == KEYPAD_HOLD_CYCLES) {
                    KeyEvent ev;
                    ev.key = keymap[r][c];
                    ev.event = KEY_EVENT_HOLD;
                    KeypadQueue_Push(ev);
                }
            }
        }
    }
}

/* ==== Public: init ==== */
void Keypad_Init(void)
{
    KeypadQueue_Init();

    /* Reset ma trận trạng thái */
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++) {
            raw[r][c] = prev[r][c] = stable[r][c] = 0;
            hold_count[r][c] = 0;
        }

    /* Kéo tất cả cột về HIGH */
    for (int i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(col_port[i], col_pin[i], GPIO_PIN_SET);
    }

    /* Bắt đầu TIM2 ngắt */
    HAL_TIM_Base_Start_IT(&htim2);
}

/* ==== Public: lấy event ==== */
bool Keypad_PollEvent(KeyEvent *out)
{
    return KeypadQueue_Pop(out);
}

void Keypad_FlushQueue(void)
{
    KeypadQueue_Clear();
}
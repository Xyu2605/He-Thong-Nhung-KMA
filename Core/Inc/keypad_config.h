#ifndef KEYPAD_CONFIG_H
#define KEYPAD_CONFIG_H

#include "main.h"

/* ==== Chân GPIO (theo bảng 2.4 báo cáo) ==== */
#define KP_C1_PORT   GPIOA
#define KP_C1_PIN    GPIO_PIN_8

#define KP_C2_PORT   GPIOB
#define KP_C2_PIN    GPIO_PIN_13

#define KP_C3_PORT   GPIOB
#define KP_C3_PIN    GPIO_PIN_14

#define KP_C4_PORT   GPIOB
#define KP_C4_PIN    GPIO_PIN_15

#define KP_R1_PORT   GPIOA
#define KP_R1_PIN    GPIO_PIN_12

#define KP_R2_PORT   GPIOA
#define KP_R2_PIN    GPIO_PIN_11

#define KP_R3_PORT   GPIOA
#define KP_R3_PIN    GPIO_PIN_10

#define KP_R4_PORT   GPIOA
#define KP_R4_PIN    GPIO_PIN_9

/* ==== Keymap ==== */
/* Hàng R1-R4 × Cột C1-C4 */
#define KEYPAD_MAP {        \
    {'1','2','3','A'},      \
    {'4','5','6','B'},      \
    {'7','8','9','C'},      \
    {'*','0','#','D'}       \
}

/* ==== Tham số debounce ==== */
#define KEYPAD_DEBOUNCE_CYCLES  2U    // 2 chu kỳ liên tiếp
#define KEYPAD_HOLD_CYCLES      50U   // 500ms → HOLD
#define KEYPAD_QUEUE_SIZE       16U   // Ring buffer

#endif
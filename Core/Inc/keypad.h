#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <stdbool.h>

/* Loại sự kiện phím */
typedef enum {
    KEY_EVENT_NONE = 0,
    KEY_EVENT_PRESSED,     // Vừa nhấn xuống
    KEY_EVENT_HOLD,        // Giữ > 500ms
    KEY_EVENT_RELEASED     // Vừa nhả ra
} KeyEventType;

/* Sự kiện phím */
typedef struct {
    char         key;       // Ký tự ASCII: '0'-'9', '*', '#', 'A'-'D'
    KeyEventType event;     // Loại sự kiện
} KeyEvent;


/* Khởi tạo GPIO + TIM2 ngắt 10ms */
void Keypad_Init(void);

/* Lấy 1 sự kiện khỏi queue (non-blocking)
 * Trả về true nếu có event, false nếu queue rỗng */
bool Keypad_PollEvent(KeyEvent *out);

/* Xóa toàn bộ queue (dùng khi đổi state) */
void Keypad_FlushQueue(void);

#endif
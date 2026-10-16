#include "keypad_queue.h"
#include "keypad_config.h"

static KeyEvent queue[KEYPAD_QUEUE_SIZE];
static volatile uint8_t head = 0;   // vị trí ghi
static volatile uint8_t tail = 0;   // vị trí đọc
static volatile uint8_t count = 0;

void KeypadQueue_Init(void) {
    head = tail = count = 0;
}

bool KeypadQueue_Push(KeyEvent ev) {
    if (count >= KEYPAD_QUEUE_SIZE) {
        return false;   // Queue đầy → bỏ event
    }
    queue[head] = ev;
    head = (head + 1) % KEYPAD_QUEUE_SIZE;
    count++;
    return true;
}

bool KeypadQueue_Pop(KeyEvent *out) {
    if (count == 0) return false;
    *out = queue[tail];
    tail = (tail + 1) % KEYPAD_QUEUE_SIZE;
    count--;
    return true;
}

void KeypadQueue_Clear(void) {
    head = tail = count = 0;
}

uint8_t KeypadQueue_Count(void) {
    return count;
}
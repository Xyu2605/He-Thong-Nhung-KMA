#ifndef KEYPAD_QUEUE_H
#define KEYPAD_QUEUE_H

#include "keypad.h"

void KeypadQueue_Init(void);
bool KeypadQueue_Push(KeyEvent ev);
bool KeypadQueue_Pop(KeyEvent *out);
void KeypadQueue_Clear(void);
uint8_t KeypadQueue_Count(void);

#endif
#ifndef SERVO_H
#define SERVO_H

#include "main.h"
#include <stdint.h>

void Servo_SetAngle(uint8_t angle);
void Door_Open(void);
void Door_Close(void);

#endif
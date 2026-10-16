#ifndef BUZZER_H
#define BUZZER_H

#include "main.h"
#include <stdbool.h>

void Buzzer_On(void);
void Buzzer_Off(void);
bool Buzzer_IsActive(void);
void Buzzer_Update(void);

#endif
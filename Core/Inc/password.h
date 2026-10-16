#ifndef PASSWORD_H
#define PASSWORD_H

#include "main.h"


extern char system_password[];

void Password_SaveFlash(void);
void Password_LoadFlash(void);

#endif
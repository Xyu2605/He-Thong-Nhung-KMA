#ifndef RFID_H
#define RFID_H

#include "main.h"
#include <stdbool.h>


void RFID_Init(void);

bool RFID_PollAdmin(void);

#endif
#ifndef APP_FSM_H
#define APP_FSM_H

#include "main.h"
#include <stdint.h>

typedef enum
{
    ST_LOGIN = 0,
    ST_DOOR_OPEN,
    ST_LOCKED,
    ST_ADMIN_OLD_PASSWORD,
    ST_ADMIN_NEW_PASSWORD
} AppState;


extern AppState app_state;
extern char     input_buffer[17];
extern uint8_t  input_len;


void ClearInput(void);
void AppendInput(char key);
void BackspaceInput(void);

/* ===== FSM ===== */
void HandleKey(char key);
void CheckSystemTimers(void);

#endif
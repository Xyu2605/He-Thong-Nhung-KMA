#ifndef APP_FSM_H
#define APP_FSM_H

#include <stdint.h>

/* ===== Định nghĩa state ===== */
typedef enum {
    STATE_LOGIN = 0,          /* Chờ nhập mật khẩu */
    STATE_DOOR_OPEN,          /* Cửa đang mở (5s) */
    STATE_LOCKED,             /* Bị khóa 30s */
    STATE_ADMIN_OLD_PW,       /* Admin: nhập MK hiện tại */
    STATE_ADMIN_NEW_PW        /* Admin: nhập MK mới */
} AppState;

/* ===== API ===== */
void     AppFSM_Init(void);
void     AppFSM_HandleKey(char key);       /* Gọi từ main khi có phím */
void     AppFSM_HandleRFID(void);          /* Gọi từ main khi quẹt thẻ admin */
void     AppFSM_Update(void);              /* Gọi mỗi vòng loop (timer) */
AppState AppFSM_GetState(void);

#endif
#ifndef APP_UI_H
#define APP_UI_H

#include <stdint.h>

/* Init LCD + hiển thị màn hình chờ */
void AppUI_Init(void);

/* Các màn hình theo state */
void AppUI_ShowLogin(void);
void AppUI_ShowInputPassword(void);                       /* Hiển thị **** */
void AppUI_ShowWrongPassword(uint8_t attempts_left);
void AppUI_ShowDoorOpen(void);
void AppUI_ShowLocked(uint32_t seconds_left);
void AppUI_ShowAdminOldPw(void);
void AppUI_ShowAdminNewPw(void);
void AppUI_ShowChangeSuccess(void);
void AppUI_ShowAdminRejected(void);

#endif
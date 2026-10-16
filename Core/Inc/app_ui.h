#ifndef APP_UI_H
#define APP_UI_H

#include "main.h"
#include "CLCD_I2C.h"

/* Biến LCD dùng chung */
extern CLCD_I2C_Name LCD1;

/* Hiển thị mật khẩu che dạng **** */
void LCD_ShowInput(const char *title);

#endif
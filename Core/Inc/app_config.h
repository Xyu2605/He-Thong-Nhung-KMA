#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#define PASSWORD_FLASH_ADDR    0x0800FC00UL
#define PASSWORD_MAGIC         0xA55AU
#define PASSWORD_MAX_LEN       8U
#define UID_LEN                4U

#define MAX_WRONG_ATTEMPTS     3U
#define LOCK_TIME_MS           30000UL
#define DOOR_OPEN_TIME_MS      5000UL
#define BUZZER_TIME_MS         3000UL
#define RFID_SCAN_PERIOD_MS    100UL

#define SERVO_CCR_0_DEG        25U
#define SERVO_CCR_90_DEG       75U
#define SERVO_CCR_180_DEG      125U

#endif
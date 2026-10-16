#include "rfid.h"
#include "RC522.h"
#include "app_config.h"
#include <string.h>


static uint8_t  status;
static uint8_t  str[16];

static uint8_t  admin_uid[UID_LEN] =
{
    0xFC, 0x31, 0x03, 0x07
};

static uint32_t last_rfid_scan = 0;


static bool RFID_IsAdmin(uint8_t *uid)
{
    for (uint8_t i = 0; i < UID_LEN; i++)
    {
        if (uid[i] != admin_uid[i])
        {
            return false;
        }
    }

    return true;
}


void RFID_Init(void)
{
    MFRC522_Init();
    last_rfid_scan = 0;
}


bool RFID_PollAdmin(void)
{
    /* Giới hạn tần số quét 100ms — giống code cũ */
    if (HAL_GetTick() - last_rfid_scan < RFID_SCAN_PERIOD_MS)
    {
        return false;
    }

    last_rfid_scan = HAL_GetTick();

    /* COPY NGUYÊN từ code cũ */
    status = MFRC522_Request(PICC_REQIDL, str);

    if (status != MI_OK)
        return false;

    status = MFRC522_Anticoll(str);

    if (status != MI_OK)
        return false;

    return RFID_IsAdmin(str);
}
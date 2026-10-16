#include "password.h"
#include "app_config.h"
#include <string.h>


char system_password[PASSWORD_MAX_LEN + 1] = "1234";


void Password_SaveFlash(void)
{
    FLASH_EraseInitTypeDef erase;
    uint32_t page_error = 0;

    HAL_FLASH_Unlock();

    erase.TypeErase   = FLASH_TYPEERASE_PAGES;
    erase.PageAddress = PASSWORD_FLASH_ADDR;
    erase.NbPages     = 1;

    if (HAL_FLASHEx_Erase(&erase, &page_error) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return;
    }

    /* Lưu MAGIC */
    HAL_FLASH_Program(
        FLASH_TYPEPROGRAM_HALFWORD,
        PASSWORD_FLASH_ADDR,
        PASSWORD_MAGIC
    );

    /* Lưu password */
    for (uint32_t i = 0; i <= PASSWORD_MAX_LEN; i += 2)
    {
        uint16_t data;

        data = (uint8_t)system_password[i];

        if (i + 1 <= PASSWORD_MAX_LEN)
        {
            data |= ((uint16_t)(uint8_t)system_password[i + 1] << 8);
        }
        else
        {
            data |= 0xFF00;
        }

        HAL_FLASH_Program(
            FLASH_TYPEPROGRAM_HALFWORD,
            PASSWORD_FLASH_ADDR + 2 + i,
            data
        );
    }

    HAL_FLASH_Lock();
}


void Password_LoadFlash(void)
{
    uint16_t magic;

    magic = *(volatile uint16_t *)PASSWORD_FLASH_ADDR;

    /* Flash chưa từng lưu password */
    if (magic != PASSWORD_MAGIC)
    {
        strcpy(system_password, "1234");
        Password_SaveFlash();
        return;
    }

    const char *flash_password =
        (const char *)(PASSWORD_FLASH_ADDR + 2);

    uint8_t i;

    for (i = 0; i < PASSWORD_MAX_LEN; i++)
    {
        system_password[i] = flash_password[i];

        if (flash_password[i] == '\0')
        {
            break;
        }
    }

    system_password[PASSWORD_MAX_LEN] = '\0';

    /* Bảo vệ nếu dữ liệu Flash lỗi */
    if (i == PASSWORD_MAX_LEN)
    {
        strcpy(system_password, "1234");
    }
}
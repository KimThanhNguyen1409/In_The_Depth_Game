#include "app_eeprom.h"
#include "buzzer.h"
#include "eeprom.h"

itd_game_score_t scores;
itd_game_setting_t settings;

typedef struct
{
    uint32_t magic_number;
    itd_game_score_t data;
    uint8_t checksum;
} itd_game_score_eeprom_t;

typedef struct
{
    uint32_t magic_number;
    itd_game_setting_t data;
    uint8_t checksum;
} itd_game_setting_eeprom_t;

#define ITD_GAME_SROCE_EEPROM_SIZE (sizeof(uint32_t) + sizeof(itd_game_score_t))
#define ITD_GAME_SETTING_EEPROM_SIZE (sizeof(uint32_t) + sizeof(itd_game_setting_t))

static uint8_t itd_game_eeprom_checksum(uint8_t* data, uint32_t size)
{
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < size; i++)
    {
        checksum += data[i];
    }
    return checksum;
}

static void itd_game_eeprom_update_checksum(uint32_t* magic_number, uint8_t* check_sum, uint32_t check_sum_size)
{
    *magic_number = ITD_GAME_EEPROM_MAGIC_NUMBER;
    *check_sum    = itd_game_eeprom_checksum((uint8_t*)magic_number, check_sum_size);
}

static bool itd_game_eeprom_is_valid(uint32_t* magic_number, uint8_t* check_sum, uint32_t check_sum_size)
{
    return (*magic_number == ITD_GAME_EEPROM_MAGIC_NUMBER &&
            (*check_sum == itd_game_eeprom_checksum((uint8_t*)magic_number, check_sum_size)));
}

/*****************************************************************************/
/* Setting                                                                    */
/*****************************************************************************/
static void itd_game_setting_set_default(itd_game_setting_t* data)
{
    data->mode       = ITD_GAME_SETTING_MODE_NOR;
    data->speed_mode = ITD_GAME_SETTING_SPEED_NOR;
    data->sound      = ITD_GAME_SETTING_SOUND_OFF;
}

bool itd_game_setting_write(itd_game_setting_t* data)
{
    itd_game_setting_eeprom_t eeprom_data;
    eeprom_data.data = *data;
    itd_game_eeprom_update_checksum(&eeprom_data.magic_number, &eeprom_data.checksum, ITD_GAME_SETTING_EEPROM_SIZE);
    return eeprom_write(EEPROM_SETTING_START_ADR, (uint8_t*)&eeprom_data, sizeof(eeprom_data)) == EEPROM_DRIVER_OK;
}
bool itd_game_setting_read(itd_game_setting_t* data)
{
    itd_game_setting_eeprom_t eeprom_data;
    uint8_t ret = eeprom_read(EEPROM_SETTING_START_ADR, (uint8_t*)&eeprom_data, sizeof(eeprom_data));
    if (ret == EEPROM_DRIVER_OK &&
        itd_game_eeprom_is_valid(&eeprom_data.magic_number, &eeprom_data.checksum, ITD_GAME_SETTING_EEPROM_SIZE))
    {
        *data = eeprom_data.data;
        BUZZER_Silent(data->sound ? BUZZER_SILENT_OFF : BUZZER_SILENT_ON);
        return true;
    }
    itd_game_setting_set_default(data);
    BUZZER_Silent(data->sound ? BUZZER_SILENT_OFF : BUZZER_SILENT_ON);
    return false;
}
/*****************************************************************************/
/* Score                                                                    */
/*****************************************************************************/
static void itd_game_score_set_default(itd_game_score_t* data)
{
    data->score_now = 0;
    data->score_1st = 0;
    data->score_2st = 0;
    data->score_3rd = 0;
}
bool itd_game_scores_write(itd_game_score_t* data)
{
    itd_game_score_eeprom_t eeprom_data;
    eeprom_data.data = *data;
    itd_game_eeprom_update_checksum(&eeprom_data.magic_number, &eeprom_data.checksum, ITD_GAME_SROCE_EEPROM_SIZE);
    return eeprom_write(EEPROM_SCORE_START_ADR, (uint8_t*)&eeprom_data, sizeof(eeprom_data)) == EEPROM_DRIVER_OK;
}
bool itd_game_scores_read(itd_game_score_t* data)
{
    itd_game_score_eeprom_t eeprom_data;
    uint8_t ret = eeprom_read(EEPROM_SCORE_START_ADR, (uint8_t*)&eeprom_data, sizeof(eeprom_data));
    if (ret == EEPROM_DRIVER_OK &&
        itd_game_eeprom_is_valid(&eeprom_data.magic_number, &eeprom_data.checksum, ITD_GAME_SROCE_EEPROM_SIZE))
    {
        *data = eeprom_data.data;
        return true;
    }
    itd_game_score_set_default(data);
    return false;
}
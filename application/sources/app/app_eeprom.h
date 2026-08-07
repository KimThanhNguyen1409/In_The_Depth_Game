#ifndef __APP_EEPROM_H__
#define __APP_EEPROM_H__

#include <stdint.h>
#include "app.h"

/**
  *****************************************************************************
  * EEPROM define address.
  *
  *****************************************************************************
  */
#define EEPROM_START_ADDR								(0X0000)
#define EEPROM_END_ADDR									(0X1000)

#define EEPROM_SCORE_START_ADR          (0x0010)
#define EEPROM_SETTING_START_ADR        (0x0100)

#define ITD_GAME_EEPROM_MAGIC_NUMBER    ((uint32_t)0x69746467)

/**
  *****************************************************************************
  *  In the depth game
  *
  *****************************************************************************
  */
/*setting data*/
#define ITD_GAME_SETTING_SOUND_ON       (1)
#define ITD_GAME_SETTING_SOUND_OFF      (0)
#define ITD_GAME_SETTING_MODE_NOR       (0)
#define ITD_GAME_SETTING_MODE_HARD      (1)
#define ITD_GAME_SETTING_MODE_EXPERT    (2)
#define ITD_GAME_SETTING_SPEED_NOR      (0)
#define ITD_GAME_SETTING_SPEED_FAST     (1)
#define ITD_GAME_SETTING_SPEED_FLASH    (2)

typedef struct{
  bool sound;
  uint8_t mode;
  uint8_t speed_mode;
} itd_game_setting_t;

/*score data*/
typedef struct {
  uint16_t score_now;
  uint16_t score_1st;
  uint16_t score_2st;
  uint16_t score_3rd;
} itd_game_score_t;

#ifdef __cplusplus
extern "C"
{
#endif
  extern itd_game_setting_t settings;
  extern bool itd_game_setting_read(itd_game_setting_t *data);
  extern bool itd_game_setting_write(itd_game_setting_t *data);

  extern itd_game_score_t scores;
  extern bool itd_game_scores_read(itd_game_score_t *data);
  extern bool itd_game_scores_write(itd_game_score_t *data);
#ifdef __cplusplus
}
#endif


#endif //__APP_EEPROM_H__

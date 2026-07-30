#ifndef _ITD_GAME_BOMB_H_
#define _ITD_GAME_BOMB_H_
#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "eeprom.h"
#include "app_eeprom.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"


#define BOMB_SIZE_BITMAP_X (18)
#define BOMB_SIZE_BITMAP_Y (10)

#define BOMB_NUMBER_MAX (7)
#define BOMB_INITAL_NUMBER (3)

#define BOMB_DESPAWN_AXIS_X (10)

#define BOMB_STEP_AXIS_X (3)

#define BOMB_SPAWN_AXIS_X_MAX (135)
#define BOMB_SPAWN_AXIS_X_MIN (128)
#define BOMB_SPAWN_AXIS_Y_MIN (15)
#define BOMB_SPAWN_AXIS_Y_MAX (42)

#define BOMB_SPAWN_LEFT_OFFSET (2)
#define BOMB_SPAWN_TOP_OFFSET (2)

typedef struct
{
    bool visible;
    uint8_t x;
    uint8_t y;
} itd_game_bomb_t;
extern uint8_t bonus_speed;
extern uint8_t bomb_number;
extern itd_game_bomb_t bombs[BOMB_NUMBER_MAX];
#endif /*_ITD_GAME_BOMB_H_*/
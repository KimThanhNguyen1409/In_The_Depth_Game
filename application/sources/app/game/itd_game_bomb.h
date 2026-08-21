#ifndef _ITD_GAME_BOMB_H_
#define _ITD_GAME_BOMB_H_

#include "fsm.h"
#include "message.h"
#include "port.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>

#include "app_eeprom.h"
#include "eeprom.h"

#include "app.h"
#include "app_dbg.h"
#include "task_display.h"
#include "task_list.h"

#define BOMB_SIZE_BITMAP_X (20)
#define BOMB_SIZE_BITMAP_Y (8)

#define BOMB_NUMBER_MAX (6)
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
    uint8_t action_image;
} itd_game_bomb_t;

extern itd_game_bomb_t bombs[BOMB_NUMBER_MAX];
extern uint8_t bonus_speed;
extern uint8_t bomb_number;

#endif //_ITD_GAME_BOMB_H_
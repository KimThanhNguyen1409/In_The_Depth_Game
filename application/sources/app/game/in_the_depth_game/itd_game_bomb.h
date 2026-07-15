#ifndef _ITD_GAME_BOMB_H_
#define _ITD_GAME_BOMB_H_
#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

#define BOMB_SIZE_BITMAP_X (12)
#define BOMB_SIZE_BITMAP_Y (12)

#define BOMB_NUMBER_MAX (7)
#define BOMB_INITAL_NUMBER (3)

#define BOMB_DESPAWN_AXIS_X (10)

#define BOMB_STEP_AXIS_X (3)

#define BOMB_SPAWN_AXIS_X_MAX (120)
#define BOMB_SPAWN_AXIS_X_MIN (110)
#define BOMB_SPAWN_AXIS_Y_MIN (20)
#define BOMB_SPAWN_AXIS_Y_MAX (50)

typedef struct
{
    bool visible;
    uint8_t x;
    uint8_t y;
} itd_game_bomb_t;

extern uint8_t bomb_number;
extern itd_game_bomb_t bombs[BOMB_NUMBER_MAX];
#endif /*_ITD_GAME_BOMB_H_*/
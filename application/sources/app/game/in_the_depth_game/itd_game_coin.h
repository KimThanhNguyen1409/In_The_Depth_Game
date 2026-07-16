#ifndef _ITD_GAME_COIN_H_
#define _ITD_GAME_COIN_H_

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

#include "itd_game_mainsub.h"

#define COIN_SIZE_BITMAP_X (16)
#define COIN_SIZE_BITMAP_Y (12)

#define COIN_INITAL_NUMBER (3)
#define COIN_NUMBER_MAX (5)

#define COIN_DESPAWN_AXIS_X (10)

#define COIN_SPAWN_AXIS_X_MAX (170)
#define COIN_SPAWN_AXIS_X_MIN (160)
#define COIN_SPAWN_AXIS_Y_MIN (20)
#define COIN_SPAWN_AXIS_Y_MAX (50)

#define COIN_SPAWN_TOP_OFFSET (2)
#define COIN_SPAWN_LEFT_OFFSET (2)

#define COIN_STEP_AXIS_X (2)

typedef struct
{
    uint8_t x;
    uint8_t y;
    bool visible;
} itd_game_coin_t;

extern uint8_t coin_number;
extern uint8_t current_coin;
extern itd_game_coin_t coins[COIN_NUMBER_MAX];

#endif
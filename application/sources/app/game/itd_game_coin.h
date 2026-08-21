#ifndef _ITD_GAME_COIN_H_
#define _ITD_GAME_COIN_H_

#include "fsm.h"
#include "message.h"
#include "port.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "app_dbg.h"
#include "task_display.h"
#include "task_list.h"

#include "itd_game_gift.h"
#include "itd_game_mainsub.h"
#include "itd_game_spike.h"

#define COIN_SIZE_BITMAP_X (16)
#define COIN_SIZE_BITMAP_Y (12)

#define COIN_INITAL_NUMBER (4)
#define COIN_NUMBER_MAX (8)

#define COIN_DESPAWN_AXIS_X (10)

#define COIN_SPAWN_AXIS_X_MAX (230)
#define COIN_SPAWN_AXIS_X_MIN (210)
#define COIN_SPAWN_AXIS_Y_MIN (20)
#define COIN_SPAWN_AXIS_Y_MAX (42)

#define COIN_SPAWN_TOP_OFFSET (2)
#define COIN_SPAWN_LEFT_OFFSET (2)

#define COIN_STEP_AXIS_X (2)

#define SAFE_DISTANCE (20)

typedef struct
{
    uint8_t x;
    uint8_t y;
    bool visible;
} itd_game_coin_t;

extern itd_game_coin_t coins[COIN_NUMBER_MAX];
extern uint8_t coin_number;
extern uint8_t current_coin;

#endif //_ITD_GAME_COIN_H_
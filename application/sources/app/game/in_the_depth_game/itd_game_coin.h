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

#define COIN_SIZE_BITMAP_X (16)
#define COIN_SIZE_BITMAP_Y (16)

#define COIN_INITAL_NUMBER 3
#define COIN_NUMBER_MAX 6

#define COIN_DESPAWN_AXIS_X (10)

#define COIN_SPAWN_AXIS_X_MAX (120)
#define COIN_SPAWN_AXIS_X_MIN (100)
#define COIN_SPAWN_AXIS_Y_MIN (20)
#define COIN_SPAWN_AXIS_Y_MAX (50)

#define COIN_STEP_AXIS_X (2)

typedef struct
{
    uint8_t x;
    uint8_t y;
    bool visible;
} itd_game_coin_t;

extern itd_game_coin_t coin[COIN_NUMBER_MAX];

#endif
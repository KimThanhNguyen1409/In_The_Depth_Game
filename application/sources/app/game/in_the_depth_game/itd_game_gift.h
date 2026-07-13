#ifndef _ITD_GAME_GIFT_H_
#define _ITD_GAME_GIFT_H_

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

#include "itd_game_bomb.h"
#include "itd_game_coin.h"
#include "itd_game_heart.h"
#include "itd_game_spike.h"

#define GIFT_SIZE_BITMAP_X (16)
#define GIFT_SIZE_BITMAP_Y (16)

#define BARRIER_BITMAP_SIZE_X (16);
#define BARRIER_BITMAP_SIZE_Y (16);

#define GIFT_NUMBER_MAX (2)

#define GIFT_NUMBER_BUFF (4)

#define GIFT_DESPAWN_AXIS_X (10)

#define GIFT_STEP_AXIS_X (2)

#define GIFT_SPAWN_AXIS_X_MAX (120)
#define GIFT_SPAWN_AXIS_X_MIN (100)
#define GIFT_SPAWN_AXIS_Y_MIN (20)
#define GIFT_SPAWN_AXIS_Y_MAX (50)

typedef enum
{
    GET_HEART,
    GET_BARRIER,
    GET_BONUS_COIN,
    GET_NUKE
} buff_type_t;

typedef struct
{
    uint8_t x;
    uint8_t y;
    bool visible;
} itd_game_gift_t;

extern uint8_t gift_number;
extern itd_game_gift_t gifts[GIFT_NUMBER_MAX];
extern uint8_t buffs;
#endif /*_ITD_GAME_GIFT_H_*/
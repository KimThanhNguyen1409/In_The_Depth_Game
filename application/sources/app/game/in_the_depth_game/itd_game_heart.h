#ifndef _ITD_GAME_HEART_
#define _ITD_GAME_HEART_

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
#include "itd_game_bomb.h"
#include "itd_game_border.h"

#define HEART_SIZE_BITMAP_X (12)
#define HEART_SIZE_BITMAP_Y (12)

#define HEART_MAX_NUMBER (3)

#define HEART_AXIS_X (16)
#define HEART_AXIS_Y (4)

#define HEART_DISTANCE_AXIS_X (6)

typedef struct
{
    uint8_t x;
    uint8_t y;
    bool visible;
} itd_game_heart_t;
extern uint8_t current_heart;
extern itd_game_heart_t hearts[HEART_MAX_NUMBER];

#endif /*_ITD_GAME_HEART_*/
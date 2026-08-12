#ifndef _ITD_GAME_BOOM_H_
#define _ITD_GAME_BOOM_H_
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

#define BOOM_NUMBER (4)

#define BOOM_BITMAP_SIZE_X (16)
#define BOOM_BITMAP_SIZE_Y (16)

#define BOOM_SPAWN_OFFSET_X (5)
#define BOOM_SPAWN_OFFSET_Y (2)

typedef struct
{
    uint8_t x;
    uint8_t y;
    bool visible;
    uint8_t action_image;
} itd_game_boom_t;

extern itd_game_boom_t boom[BOOM_NUMBER];

extern void itd_game_boom_spawn(uint16_t x, uint16_t y);

#endif
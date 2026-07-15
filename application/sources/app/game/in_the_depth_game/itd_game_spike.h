#ifndef _ITD_GAME_SPIKE_H_
#define _ITD_GAME_SPIKE_H_

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

#define SPIKE_TRIPLE_SIZE_BITMAP_X (32)
#define SPIKE_TRIPLE_SIZE_BITMAP_Y (16)

#define SPIKE_SINGLE_SIZE_BITMAP_X (16)
#define SPIKE_SINGLE_SIZE_BITMAP_Y (32)

#define SPIKE_TRIPLE_TYPE (3)
#define SPIKE_SINGLE_TYPE (2)

#define SPIKE_NUMBER (3)

#define SPIKE_TRIPLE_SPAWN_X (125)
#define SPIKE_SINGLE_SPAWN_X (125)
#define SPIKE_TRIPLE_SPAWN_Y (50)
#define SPIKE_SINGLE_SPAWN_Y (40)
#define SPIKE_STEP_X (2)

#define SPIKE_DESPAWN_AXIS_X (10)

typedef struct
{
    uint8_t x;
    uint8_t y;
    uint8_t type;
    bool visible;
} itd_game_spike_t;

extern itd_game_spike_t spikes[SPIKE_NUMBER];

#endif
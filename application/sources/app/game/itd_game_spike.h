#ifndef _ITD_GAME_SPIKE_H_
#define _ITD_GAME_SPIKE_H_

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

#define SPIKE_SHORT_SIZE_BITMAP_X (16)
#define SPIKE_SHORT_SIZE_BITMAP_Y (16)

#define SPIKE_TALL_SIZE_BITMAP_X (16)
#define SPIKE_TALL_SIZE_BITMAP_Y (32)

#define SPIKE_SHORT_TYPE (3)
#define SPIKE_TALL_TYPE (2)

#define SPIKE_NUMBER (3)

#define SPIKE_SHORT_SPAWN_X (190)
#define SPIKE_SHORT_SPAWN_Y (42)
#define SPIKE_TALL_SPAWN_X (210)
#define SPIKE_TALL_SPAWN_Y (30)
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
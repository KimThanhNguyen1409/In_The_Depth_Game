#ifndef _ITD_GAME_MAINSUB_H_
#define _ITD_GAME_MAINSUB_H_

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
#include "itd_game_boom.h"
#include "itd_game_spike.h"
#include "itd_game_coin.h"
#include "itd_game_heart.h"
#include "itd_game_gift.h"

#define MAINSUB_SIZE_BITMAP_X (20)
#define MAINSUB_SIZE_BITMAP_Y (16)

#define MAINSUB_AXIS_X (16)
#define MAINSUB_AXIS_Y (32)

#define MAINSUB_STEP_AXIS_Y (4)

#define MAINSUB_HITBOX_LEFT_OFFSET (11)
#define MAINSUB_HITBOX_RIGHT_OFFSET (17)
#define MAINSUB_HITBOX_TOP_OFFSET (1)
#define MAINSUB_HITBOX_BOTTOM_OFFSET (9)

#define MAINSUB_AXIS_Y_MAX (55)
#define MAINSUB_AXIS_Y_MIN (15)

typedef struct
{
    uint8_t x;
    uint8_t y;
    bool visible;
    uint8_t shield_heart = 0;
    uint8_t invincibility_time = 0;
    // buff_type_t applied_buff; 
} itd_game_mainsub_t;

extern itd_game_mainsub_t mainsub;

extern bool itd_game_mainsub_check_hit_by_bomb(uint8_t bo);
extern bool itd_game_mainsub_check_hit_by_spike(uint8_t sp, uint8_t type);
extern bool itd_game_mainsub_check_get_coin(uint8_t co);
extern bool itd_game_mainsub_check_get_gift(uint8_t gi);
#endif /*_ITD_GAME_MAINSUB_H_*/
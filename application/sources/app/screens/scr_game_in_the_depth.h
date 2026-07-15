#ifndef _SCR_GAME_IN_THE_DEPTH_H_
#define _SCR_GAME_IN_THE_DEPTH_H_

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
#include "view_render.h"

#include "itd_game_bomb.h"
#include "itd_game_boom.h"
// #include "itd_game_border.h"
#include "itd_game_coin.h"
// #include "itd_game_heart.h"
#include "itd_game_mainsub.h"
#include "itd_game_spike.h"
// #include "itd_game_gift.h"

#define GAME_START (0)
#define GAME_PLAY (1)
#define GAME_OVER (2)

extern view_dynamic_t dyn_view_in_the_depth;
extern view_screen_t scr_game_in_the_depth;
extern void scr_game_in_the_depth_handle(ak_msg_t *msg);

extern void itd_game_mainsub_display();
extern void itd_game_bomb_display();
extern void itd_game_boom_display();
extern void itd_game_spike_display();
extern void itd_game_coin_display();
extern void itd_game_heart_display();

#endif /*_SCR_GAME_IN_THE_DEPTH_H_*/
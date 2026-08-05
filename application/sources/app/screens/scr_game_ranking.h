#ifndef _SCR_GAME_RANKING_H_
#define _SCR_GAME_RANKING_H_

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

#include "screens.h"
#include "screens_bitmap.h"
#include "scr_game_menu.h"

#define CHAIN_BITMAP_AXIS_X (10)
#define CHAIN_BITMAP_AXIS_Y (40)

extern view_dynamic_t dyn_view_item_game_ranking;
extern view_screen_t scr_game_ranking;
extern void scr_game_ranking_handle(ak_msg_t* msg);

#endif /*_SCR_GAME_RANKING_H_*/
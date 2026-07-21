#ifndef _SCR_GAME_OVER_H_
#define _SCR_GAME_OVER_H_

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

#include "scr_game_in_the_depth.h"
#include "scr_game_menu.h"
extern view_dynamic_t dyn_view_item_game_over;
extern view_screen_t scr_game_over;
extern void scr_game_over_handle(ak_msg_t *msg);

#endif /*_SCR_GAME_OVER_H_*/
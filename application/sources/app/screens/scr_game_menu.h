#ifndef _SCR_GAME_MENU_H_
#define _SCR_GAME_MENU_H_

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
#include "scr_game_setting.h"


#define MENU_LOCATION_1 (0)
#define MENU_LOCATION_2 (1)
#define MENU_LOCATION_3 (2)
#define MENU_LOCATION_4 (3)


extern view_dynamic_t dyn_view_game_menu;
extern view_screen_t scr_game_menu;
extern void scr_game_menu_handle(ak_msg_t *msg);


#endif /*_SCR_GAME_MENU_H_*/
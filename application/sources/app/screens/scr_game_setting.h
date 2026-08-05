#ifndef _SCR_GAME_SETTING_H_
#define _SCR_GAME_SETTING_H_

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

#include "eeprom.h"
#include "app_eeprom.h"

#include "screens.h"
#include "screens_bitmap.h"

#include "scr_game_in_the_depth.h"
#include "scr_game_menu.h"
#include "itd_game_mainsub.h"
#include "itd_game_bomb.h"

#define AC_DISPLAY_ROUNDRECT_FRAME_W (112)
#define AC_DISPLAY_ROUNDRECT_FRAME_H (16)

#define AC_DISPLAY_ROUNDRECT_CHOOSE_W (110)
#define AC_DISPLAY_ROUNDRECT_CHOOSE_H (14)

#define AC_DISPLAY_ROUNDRECT_RADIUS (3)

#define AC_DISPLAY_ROUNDRECT_AXIS_X (8)
#define AC_DISPLAY_TEXT_AXIS_X (12)

#define SETTING_LOCATION_1 (0)
#define SETTING_LOCATION_2 (1)
#define SETTING_LOCATION_3 (2)
#define SETTING_LOCATION_4 (3)

extern view_dynamic_t dyn_view_game_setting;
extern view_screen_t scr_game_setting;
extern void scr_game_setting_handle(ak_msg_t* msg);

#endif /*_SCR_GAME_SETTING_H_*/
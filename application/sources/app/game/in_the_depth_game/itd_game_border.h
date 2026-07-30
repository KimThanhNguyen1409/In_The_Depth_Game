#ifndef _ITD_GAME_BORDER_H_
#define _ITD_GAME_BORDER_H_

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
#include "itd_game_heart.h"
#include "itd_game_coin.h"

extern uint16_t itd_game_score;
extern uint16_t itd_game_time;
extern uint8_t itd_game_heart;
extern uint8_t itd_game_shield;
extern uint16_t time_last;
#endif /*_ITD_GAME_BORDER_H_*/
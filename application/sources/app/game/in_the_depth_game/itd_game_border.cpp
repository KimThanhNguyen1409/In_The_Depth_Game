#include "itd_game_border.h"

static bool is_game_over = false;
uint16_t itd_game_score = 0;
uint16_t itd_game_time = 0;
uint8_t itd_game_heart = 0;
uint8_t itd_game_shield = 0;
uint16_t time_last;
void itd_game_border_reset_all()
{
	itd_game_score = 0;
	itd_game_time = 0;
	itd_game_heart = 3;
	itd_game_shield = 0;
	is_game_over = false;
}

void itd_game_border_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case ITD_GAME_BORDER_SETUP:
	{
		APP_DBG_SIG("ITD_GAME_BORDER_SETUP");
		itd_game_border_reset_all();
	}
	break;
	case ITD_GAME_BORDER_CHECK_GAME_OVER:
	{
		APP_DBG_SIG("ITD_GAME_BORDER_GAME_OVER");
		if (itd_game_heart == 0 && is_game_over == false)
		{
			is_game_over = true;
			task_post_pure_msg(AC_TASK_DISPLAY_ID, ITD_GAME_RESET);
		}
	}
	break;
	case ITD_GAME_BORDER_UPDATE:
	{
		APP_DBG_SIG("ITD_GAME_BORDER_UPDATE");
		itd_game_score = 10 * current_coin;
		itd_game_heart = mainsub.current_heart;
		itd_game_shield = mainsub.shield_heart;
		itd_game_time++;
	}
	break;
	case ITD_GAME_BORDER_RESET:
	{
		APP_DBG_SIG("ITD_GAME_BORDER_RESET");
		itd_game_border_reset_all();
	}
	break;
	default:
		break;
	}
}
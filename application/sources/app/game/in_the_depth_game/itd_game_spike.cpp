#include "itd_game_spike.h"

itd_game_spike_t spikes[SPIKE_NUMBER];

static void itd_game_spike_reset_all()
{
	for (uint8_t i = 0; i < SPIKE_NUMBER; i++)
	{
		spikes[i].x = 0;
		spikes[i].y = 0;
		spikes[i].type = 0;
		spikes[i].visible = BLACK;
	}
}

/*-----------------------------Spike handle-----------------------------*/
void itd_game_spike_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case ITD_GAME_SPIKE_SETUP:
	{
		APP_DBG_SIG("ITD_GAME_SPIKE_SETUP");
		itd_game_spike_reset_all();
	}
	break;
	case ITD_GAME_SPIKE_SPAWN:
	{
		APP_DBG_SIG("ITD_GAME_SPIKE_SPAWN");
		for (uint8_t i = 0; i < SPIKE_NUMBER; i++)
		{
			if (spikes[i].visible == WHITE)
				continue;
			uint8_t type = 2 + rand() % 2;
			if (type == 3)
			{
				spikes[i].type = SPIKE_SHORT_TYPE;
				spikes[i].x = SPIKE_SHORT_SPAWN_X;
				spikes[i].y = SPIKE_SHORT_SPAWN_Y;
				spikes[i].visible = WHITE;
			}
			else if (type == 2)
			{
				spikes[i].type = SPIKE_TALL_TYPE;
				spikes[i].x = SPIKE_TALL_SPAWN_X;
				spikes[i].y = SPIKE_TALL_SPAWN_Y;
				spikes[i].visible = WHITE;
			}
		}
		for (uint8_t i = 0; i < SPIKE_NUMBER; i++)
		{
			if (spikes[i].visible != WHITE)
				continue;
			if (spikes[i].x <= SPIKE_DESPAWN_AXIS_X + SPIKE_STEP_X)
			{
				spikes[i].visible = BLACK;
				spikes[i].x = 0;
			}
			else
			{
				spikes[i].x -= SPIKE_STEP_X;
			}
		}
	}
	break;
	case ITD_GAME_SPIKE_RESET:
	{
		APP_DBG_SIG("ITD_GAME_SPIKE_RESET");
		itd_game_spike_reset_all();
	}
	break;
	default:
		break;
	}
}
#include "itd_game_coin.h"

itd_game_coin_t coins[COIN_NUMBER_MAX];
uint8_t coin_number;
uint8_t current_coin = 0;
static void itd_game_coin_reset_all()
{
	current_coin = 0;
	for (uint8_t i = 0; i < COIN_NUMBER_MAX; i++)
	{
		coins[i].x = 0;
		coins[i].y = 0;
		coins[i].visible = BLACK;
	}
}
static bool check_spawn_overlap(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	for (uint8_t i = 0; i < GIFT_NUMBER_MAX; i++)
	{
		if (gifts[i].visible == WHITE)
		{
			if ((x + w > gifts[i].x) && (x < gifts[i].x + GIFT_SIZE_BITMAP_X) &&
			    (y + h > gifts[i].y) && (y < gifts[i].y + GIFT_SIZE_BITMAP_Y))
			{
				return true;
			}
		}
	}
	for (uint8_t i = 0; i < COIN_NUMBER_MAX; i++)
	{
		if (coins[i].visible == WHITE)
		{
			if ((x + w > coins[i].x) && (x < coins[i].x + COIN_SIZE_BITMAP_X) &&
			    (y + h > coins[i].y) && (y < coins[i].y + COIN_SIZE_BITMAP_Y))
			{
				return true;
			}
		}
	}
	for (uint8_t i = 0; i < SPIKE_NUMBER; i++)
	{
		if (spikes[i].visible == WHITE)
		{
			uint8_t spike_w = (spikes[i].type == 2) ? SPIKE_TALL_SIZE_BITMAP_X : SPIKE_SHORT_SIZE_BITMAP_X;
			uint8_t spike_h = (spikes[i].type == 2) ? SPIKE_TALL_SIZE_BITMAP_Y : SPIKE_SHORT_SIZE_BITMAP_Y;

			if ((x + w > spikes[i].x) && (x < spikes[i].x + spike_w) &&
			    (y + h > spikes[i].y) && (y < spikes[i].y + spike_h))
				return true;
		}
	}

	return false;
}
void itd_game_coin_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case ITD_GAME_COIN_SETUP:
	{
		APP_DBG_SIG("ITD_GAME_COIN_SETUP");
		itd_game_coin_reset_all();
		coin_number = COIN_INITAL_NUMBER + rand() % (COIN_NUMBER_MAX - COIN_INITAL_NUMBER + 1);
	}
	break;
	case ITD_GAME_COIN_SPAWN:
	{
		APP_DBG_SIG("ITD_GAME_COIN_SPAWN");
		static uint8_t coin_cooldown = 0;
		if (coin_cooldown > 0)
		{
			coin_cooldown--;
		}
		else
		{
			for (uint8_t i = 0; i < coin_number; i++)
			{
				if (coins[i].visible == WHITE)
					continue;
				uint8_t temp_x, temp_y;
				bool is_overlapped;
				uint8_t attempts = 0;
				do
				{
					temp_x = COIN_SPAWN_AXIS_X_MIN + rand() % (COIN_SPAWN_AXIS_X_MAX - COIN_SPAWN_AXIS_X_MIN) + COIN_SPAWN_LEFT_OFFSET;
					temp_y = COIN_SPAWN_AXIS_Y_MIN + rand() % (COIN_SPAWN_AXIS_Y_MAX - COIN_SPAWN_AXIS_Y_MIN) + COIN_SPAWN_TOP_OFFSET;
					is_overlapped = check_spawn_overlap(temp_x, temp_y, COIN_SIZE_BITMAP_X, COIN_SIZE_BITMAP_Y);
					attempts++;
				} while (is_overlapped && attempts < 5);
				if (!is_overlapped)
				{
					coins[i].x = temp_x;
					coins[i].y = temp_y;
					coins[i].visible = WHITE;
					coin_cooldown = 8 + rand() % 8;
				}
				else
				{
					coin_cooldown = 2;
				}
				break;
			}
		}
		for (uint8_t i = 0; i < COIN_NUMBER_MAX; i++)
		{
			if (coins[i].visible != WHITE)
				continue;
			if (coins[i].x <= COIN_DESPAWN_AXIS_X + COIN_STEP_AXIS_X)
			{
				coins[i].x = 0;
				coins[i].visible = BLACK;
			}
			else
			{
				coins[i].x -= COIN_STEP_AXIS_X;
			}
		}
	}
	break;
	case ITD_GAME_COIN_RESET:
	{
		APP_DBG_SIG("ITD_GAME_COIN_RESET");
		itd_game_coin_reset_all();
	}
	break;
	default:
		break;
	}
}
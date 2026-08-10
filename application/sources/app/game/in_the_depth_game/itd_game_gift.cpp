#include "itd_game_gift.h"

itd_game_gift_t gifts[GIFT_NUMBER_MAX];
uint8_t gift_number;

void itd_game_gift_restet_all()
{
	for (uint8_t i = 0; i < GIFT_NUMBER_MAX; i++)
	{
		gifts[i].x = 0;
		gifts[i].y = 0;
		gifts[i].visible = BLACK;
	}
}

/*Check overlapped*/
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

/*-----------------------------Gift handle-----------------------------*/
void itd_game_gift_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case ITD_GAME_GIFT_SETUP:
	{
		APP_DBG_SIG("ITD_GAME_GIFT_SETUP");
		itd_game_gift_restet_all();
		gift_number = GIFT_INITAL_NUMBER + rand() % (GIFT_NUMBER_MAX - GIFT_INITAL_NUMBER + 1);
	}
	break;
	case ITD_GAME_GIFT_SPAWN:
	{
		APP_DBG_SIG("ITD_GAME_GIFT_SPAWN");
		static uint8_t gift_cooldown = 0;
		if (gift_cooldown > 0)
			gift_cooldown--;
		else
		{
			for (uint8_t i = 0; i < gift_number; i++)
			{
				if (gifts[i].visible == WHITE)
					continue;
				uint8_t temp_x, temp_y;
				bool is_overlapped;
				uint8_t attempts = 0;
				do
				{
					temp_x = GIFT_SPAWN_AXIS_X_MIN + rand() % (GIFT_SPAWN_AXIS_X_MAX - GIFT_SPAWN_AXIS_X_MIN) + GIFT_SPAWN_LEFT_OFFSET;
					temp_y = GIFT_SPAWN_AXIS_Y_MIN + rand() % (GIFT_SPAWN_AXIS_Y_MAX - GIFT_SPAWN_AXIS_Y_MIN) + GIFT_SPAWN_TOP_OFFSET;
					is_overlapped = check_spawn_overlap(temp_x, temp_y, GIFT_SIZE_BITMAP_X, GIFT_SIZE_BITMAP_Y);
					attempts++;
				} while (is_overlapped && attempts < 5);
				if (!is_overlapped)
				{
					gifts[i].x = temp_x;
					gifts[i].y = temp_y;
					gifts[i].visible = WHITE;
					gifts[i].buff = (buff_type_t)(rand() % 4);
					gift_cooldown = 8 + rand() % 8;
				}
				else
				{
					gift_cooldown = 2;
				}
				break;
			}
		}
		for (uint8_t i = 0; i < GIFT_NUMBER_MAX; i++)
		{
			if (gifts[i].visible != WHITE)
				continue;
			if (gifts[i].x <= GIFT_DESPAWN_AXIS_X + GIFT_STEP_AXIS_X)
			{
				gifts[i].x = 0;
				gifts[i].visible = BLACK;
			}
			else
			{
				gifts[i].x -= GIFT_STEP_AXIS_X;
			}
		}
	}
	break;
	case ITD_GAME_GIFT_UPDATE:
	{
		for (uint8_t i = 0; i < GIFT_NUMBER_MAX; i++)
		{
			if (gifts[i].visible != WHITE)
				continue;
			if (!itd_game_mainsub_check_get_gift(i))
				continue;
			last_recieved_buff = gifts[i].buff;
			buff_icon_display = 20;
			switch (gifts[i].buff)
			{
			case GET_HEART:
			{
				if (mainsub.current_heart < HEART_MAX_NUMBER)
				{
					mainsub.current_heart++;
				}
				BUZZER_PlaySound(BUZZER_SOUND_GET_HEART);
			}
			break;
			case GET_SHIELD:
			{
				if (mainsub.shield_heart == 0 || mainsub.shield_heart < 3)
				{
					mainsub.shield_heart++;
				}
				BUZZER_PlaySound(BUZZER_SOUND_GET_SHIELD);
			}
			break;
			case GET_BONUS_COIN:
			{
				current_coin += 10;
				BUZZER_PlaySound(BUZZER_SOUND_GET_BONUS_COIN);
			}
			break;
			case GET_NUKE:
			{

				BUZZER_PlaySound(BUZZER_SOUND_GET_NUKE);
				for (uint8_t i = 0; i < BOMB_NUMBER_MAX; i++)
				{
					itd_game_boom_spawn(bombs[i].x, bombs[i].y);
				}
				task_post_pure_msg(ITD_GAME_BOMB_ID, ITD_GAME_BOMB_RESET);
				for (uint8_t i = 0; i < SPIKE_NUMBER; i++)
				{
					itd_game_boom_spawn(spikes[i].x, spikes[i].y);
				}
				task_post_pure_msg(ITD_GAME_SPIKE_ID, ITD_GAME_SPIKE_RESET);
			}
			break;
			default:
				break;
			}
			gifts[i].visible = BLACK;
			gifts[i].x = 0;
		}
	}
	break;
	case ITD_GAME_GIFT_RESET:
	{
		APP_DBG_SIG("ITD_GAME_GIFT_RESET");
		itd_game_gift_restet_all();
	}
	break;
	default:
		break;
	}
}
#include "itd_game_mainsub.h"

itd_game_mainsub_t mainsub;
uint8_t last_recieved_buff;
uint8_t buff_icon_display;
bool itd_game_mainsub_check_hit_by_bomb(uint8_t bo)
{
	if (mainsub.invincibility_time > 0)
	{
		return false;
	}
	return ((((int16_t)bombs[bo].x) + BOMB_SIZE_BITMAP_X > mainsub.x + MAINSUB_HITBOX_LEFT_OFFSET) &&
	        (((int16_t)bombs[bo].x) < mainsub.x + MAINSUB_HITBOX_RIGHT_OFFSET) &&
	        (((int16_t)bombs[bo].y) + BOMB_SIZE_BITMAP_Y > mainsub.y + MAINSUB_HITBOX_TOP_OFFSET) &&
	        (((int16_t)bombs[bo].y) < mainsub.y + MAINSUB_HITBOX_BOTTOM_OFFSET));
}
bool itd_game_mainsub_check_hit_by_spike(uint8_t sp, uint8_t type)
{
	if (mainsub.invincibility_time > 0)
	{
		return false;
	}
	if (type == 2)
	{
		return (((int16_t)spikes[sp].x + SPIKE_TALL_SIZE_BITMAP_X > mainsub.x + MAINSUB_HITBOX_LEFT_OFFSET) &&
		        ((int16_t)spikes[sp].x < mainsub.x + MAINSUB_HITBOX_RIGHT_OFFSET) &&
		        ((int16_t)spikes[sp].y + SPIKE_TALL_SIZE_BITMAP_Y > mainsub.y + MAINSUB_HITBOX_TOP_OFFSET) &&
		        ((int16_t)spikes[sp].y < mainsub.y + MAINSUB_HITBOX_BOTTOM_OFFSET));
	}
	else
	{
		return (((int16_t)spikes[sp].x + SPIKE_SHORT_SIZE_BITMAP_X > mainsub.x + MAINSUB_HITBOX_LEFT_OFFSET) &&
		        ((int16_t)spikes[sp].x < mainsub.x + MAINSUB_HITBOX_RIGHT_OFFSET) &&
		        ((int16_t)spikes[sp].y + SPIKE_SHORT_SIZE_BITMAP_Y > mainsub.y + MAINSUB_HITBOX_TOP_OFFSET) &&
		        ((int16_t)spikes[sp].y < mainsub.y + MAINSUB_HITBOX_BOTTOM_OFFSET));
	}
	return 0;
}
bool itd_game_mainsub_check_get_coin(uint8_t co)
{
	return ((((int16_t)coins[co].x) + COIN_SIZE_BITMAP_X > mainsub.x + MAINSUB_HITBOX_LEFT_OFFSET) &&
	        (((int16_t)coins[co].x) < mainsub.x + MAINSUB_HITBOX_RIGHT_OFFSET) &&
	        (((int16_t)coins[co].y) + COIN_SIZE_BITMAP_Y > mainsub.y + MAINSUB_HITBOX_TOP_OFFSET) &&
	        (((int16_t)coins[co].y) < mainsub.y + MAINSUB_HITBOX_BOTTOM_OFFSET));
}
bool itd_game_mainsub_check_get_gift(uint8_t gi)
{
	return ((((int16_t)gifts[gi].x) + GIFT_SIZE_BITMAP_X > mainsub.x + MAINSUB_HITBOX_LEFT_OFFSET) &&
	        (((int16_t)gifts[gi].x) < mainsub.x + MAINSUB_HITBOX_RIGHT_OFFSET) &&
	        (((int16_t)gifts[gi].y) + GIFT_SIZE_BITMAP_Y > mainsub.y + MAINSUB_HITBOX_TOP_OFFSET) &&
	        (((int16_t)gifts[gi].y) < mainsub.y + MAINSUB_HITBOX_BOTTOM_OFFSET));
}
void itd_game_mainsub_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case ITD_GAME_MAINSUB_SETUP:
	{
		APP_DBG_SIG("ITD_GAME_MAINSUB_SETUP");
		if (settings.mode == ITD_GAME_SETTING_MODE_NOR)
		{
			mainsub.damage_taken = 1;
		}
		else if (settings.mode == ITD_GAME_SETTING_MODE_HARD)
		{
			mainsub.damage_taken = 2;
		}
		else
		{
			mainsub.damage_taken = 3;
		}
		mainsub.x = MAINSUB_AXIS_X;
		mainsub.y = MAINSUB_AXIS_Y;
		mainsub.visible = WHITE;
		mainsub.shield_heart = 0;
		mainsub.invincibility_time = 0;
		mainsub.current_heart = HEART_MAX_NUMBER;
	}
	break;
	case ITD_GAME_MAINSUB_GO_DOWN:
	{
		APP_DBG_SIG("ITD_GAME_MAINSUB_GO_DOWN");
		if (mainsub.y < MAINSUB_AXIS_Y_MAX)
		{
			mainsub.y += MAINSUB_STEP_AXIS_Y;
		}
		else
		{
			mainsub.y = MAINSUB_AXIS_Y_MAX;
		}
	}
	break;
	case ITD_GAME_MAINSUB_GO_UP:
	{
		APP_DBG_SIG("ITD_GAME_MAINSUB_GO_UP");
		if (mainsub.y > MAINSUB_AXIS_Y_MIN)
		{
			mainsub.y -= MAINSUB_STEP_AXIS_Y;
		}
		else
		{
			mainsub.y = MAINSUB_AXIS_Y_MIN;
		}
	}
	break;
	case ITD_GAME_MAINSUB_UPDATE:
	{
		APP_DBG_SIG("ITD_GAME_MAINSUB_UPDATE");
		if (mainsub.invincibility_time > 0)
		{
			mainsub.invincibility_time--;
			if (mainsub.invincibility_time % 4 == 0)
			{
				mainsub.visible = (mainsub.visible == WHITE) ? BLACK : WHITE;
			}
		}
		else
		{
			mainsub.visible = WHITE;
		}
		if (mainsub.invincibility_time == 0)
		{
			for (uint8_t i = 0; i < BOMB_NUMBER_MAX; i++)
			{
				if (bombs[i].visible == WHITE && itd_game_mainsub_check_hit_by_bomb(i))
				{
					mainsub.invincibility_time = 60;
					bombs[i].visible = BLACK;
					itd_game_boom_spawn(mainsub.x, mainsub.y);
					BUZZER_PlaySound(BUZZER_SOUND_BANG);
					mainsub.visible = BLACK;
					if (mainsub.shield_heart != 0)
					{
						if (mainsub.shield_heart >= mainsub.damage_taken)
						{
							mainsub.shield_heart -= mainsub.damage_taken;
						}
						else
						{
							uint8_t remaining_damage = mainsub.damage_taken - mainsub.shield_heart;
							mainsub.shield_heart = 0;
							if (mainsub.current_heart >= remaining_damage)
							{
								mainsub.current_heart -= remaining_damage;
							}
							else
							{
								mainsub.current_heart = 0;
							}
						}
						BUZZER_PlaySound(BUZZER_SOUND_BANG);
					}
					else if (mainsub.current_heart > 0)
					{
						if (mainsub.current_heart >= mainsub.damage_taken)
						{
							mainsub.current_heart -= mainsub.damage_taken;
						}
						else
						{
							mainsub.current_heart = 0;
						}
						BUZZER_PlaySound(BUZZER_SOUND_BANG);
					}
					break;
				}
			}
			for (uint8_t i = 0; i < SPIKE_NUMBER; i++)
			{
				if (spikes[i].visible == WHITE && itd_game_mainsub_check_hit_by_spike(i, spikes[i].type))
				{
					mainsub.invincibility_time = 60;
					itd_game_boom_spawn(mainsub.x, mainsub.y);
					BUZZER_PlaySound(BUZZER_SOUND_BANG);
					mainsub.visible = BLACK;
					if (mainsub.shield_heart != 0)
					{
						if (mainsub.shield_heart >= mainsub.damage_taken)
						{
							mainsub.shield_heart -= mainsub.damage_taken;
						}
						else
						{
							uint8_t remaining_damage = mainsub.damage_taken - mainsub.shield_heart;
							mainsub.shield_heart = 0;
							if (mainsub.current_heart >= remaining_damage)
							{
								mainsub.current_heart -= remaining_damage;
							}
							else
							{
								mainsub.current_heart = 0;
							}
						}
						BUZZER_PlaySound(BUZZER_SOUND_BANG);
					}
					else if (mainsub.current_heart > 0)
					{
						if (mainsub.current_heart >= mainsub.damage_taken)
						{
							mainsub.current_heart -= mainsub.damage_taken;
						}
						else
						{
							mainsub.current_heart = 0;
						}
						BUZZER_PlaySound(BUZZER_SOUND_BANG);
					}
					break;
				}
			}
		}
		for (uint8_t i = 0; i < COIN_NUMBER_MAX; i++)
		{
			if (coins[i].visible != WHITE)
				continue;
			if (!itd_game_mainsub_check_get_coin(i))
				continue;
			BUZZER_PlaySound(BUZZER_SOUND_COIN_COLLECT);
			coins[i].visible = BLACK;
			coins[i].x = 0;
			current_coin++;
		}
		for (uint8_t i = 0; i < GIFT_NUMBER_MAX; i++)
		{
			if (gifts[i].visible != WHITE)
				continue;
			if (!itd_game_mainsub_check_get_gift(i))
				continue;
			gifts[i].visible = BLACK;
			gifts[i].x = 0;
		}
	}
	break;
	case ITD_GAME_MAINSUB_RESET:
	{
		APP_DBG_SIG("ITD_GAME_MAINSUB_RESET");
		mainsub.x = MAINSUB_AXIS_X;
		mainsub.y = MAINSUB_AXIS_Y;
		mainsub.visible = BLACK;
	}
	break;
	default:
		break;
	}
}

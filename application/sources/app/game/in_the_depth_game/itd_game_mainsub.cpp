#include "itd_game_mainsub.h"

itd_game_mainsub_t mainsub;
uint8_t damage_taken = 0;
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
        return (((int16_t)spikes[sp].x + SPIKE_SINGLE_SIZE_BITMAP_X > mainsub.x + MAINSUB_HITBOX_LEFT_OFFSET) &&
                ((int16_t)spikes[sp].x < mainsub.x + MAINSUB_HITBOX_RIGHT_OFFSET) &&
                ((int16_t)spikes[sp].y + SPIKE_SINGLE_SIZE_BITMAP_Y > mainsub.y + MAINSUB_HITBOX_TOP_OFFSET) &&
                ((int16_t)spikes[sp].y < mainsub.y + MAINSUB_HITBOX_BOTTOM_OFFSET));
    }
    else
    {
        return (((int16_t)spikes[sp].x + SPIKE_TRIPLE_SIZE_BITMAP_X > mainsub.x + MAINSUB_HITBOX_LEFT_OFFSET) &&
                ((int16_t)spikes[sp].x < mainsub.x + MAINSUB_HITBOX_RIGHT_OFFSET) &&
                ((int16_t)spikes[sp].y + SPIKE_TRIPLE_SIZE_BITMAP_Y > mainsub.y + MAINSUB_HITBOX_TOP_OFFSET) &&
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
void itd_game_mainsub_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case ITD_GAME_MAINSUB_SETUP:
    {
        APP_DBG_SIG("ITD_GAME_MAINSUB_SETUP");
        mainsub.x = MAINSUB_AXIS_X;
        mainsub.y = MAINSUB_AXIS_Y;
        mainsub.visible = WHITE;
        mainsub.shield_heart = 0;
        mainsub.invincibility_time = 0;
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
            for (uint8_t i = 0; i < bomb_number; i++)
            {
                if (bombs[i].visible == WHITE && itd_game_mainsub_check_hit_by_bomb(i))
                {
                    mainsub.invincibility_time = 60;
                    bombs[i].visible = BLACK;
                    itd_game_boom_spawn(mainsub.x, mainsub.y);
                    mainsub.visible = BLACK;
                    if (mainsub.shield_heart != 0){
                        mainsub.shield_heart -= mainsub.damage_taken;
                    }
                    else if (current_heart > 0){
                        current_heart -= mainsub.damage_taken;
                        if(current_heart < 0)
                            current_heart = 0;
                    }
                    break;
                }
            }
            for (uint8_t i = 0; i < SPIKE_NUMBER; i++)
            {
                if (spikes[i].visible == WHITE && itd_game_mainsub_check_hit_by_spike(i, spikes[i].type))
                {
                    mainsub.invincibility_time = 60;
                    spikes[i].visible = BLACK;
                    itd_game_boom_spawn(mainsub.x, mainsub.y);
                    mainsub.visible = BLACK;
                    if (mainsub.shield_heart != 0){
                        mainsub.shield_heart -= mainsub.damage_taken;
                    }   
                    else if (current_heart > 0){
                        current_heart -= mainsub.damage_taken;
                        if(current_heart < 0)
                            current_heart = 0;
                    }
                    break;
                }
            }
        }
        for (uint8_t i = 0; i < coin_number; i++)
        {
            if (coins[i].visible != WHITE)
                continue;
            if (!itd_game_mainsub_check_get_coin(i))
                continue;
            coins[i].visible = BLACK;
            coins[i].x = 0;
            current_coin++;
        }
        for (uint8_t i = 0; i < gift_number; i++)
        {
            if (gifts[i].visible != WHITE)
                continue;
            if (!itd_game_mainsub_check_get_gift(i))
                continue;
            gifts[i].visible = BLACK;
            gifts[i].x = 0;
            switch (gifts[i].buff)
            {
            case GET_HEART:
            {
                APP_DBG_SIG("ITD_GAME_MAINSUB_GET_HEART");
                if (current_heart < 3)
                {
                    current_heart++;
                }
            }
            break;
            case GET_SHIELD:
            {
                APP_DBG_SIG("ITD_GAME_MAINSUB_GET_SHIELD");
                if (mainsub.shield_heart == 0 || mainsub.shield_heart < 3)
                {
                    mainsub.shield_heart++;
                }
            }
            break;
            case GET_BONUS_COIN:
            {
                APP_DBG_SIG("ITD_GAME_MAINSUB_GET_BONUS_COIN");
                current_coin += 10;
            }
            break;
            case GET_NUKE:
            {
                APP_DBG_SIG("ITD_GAME_MAINSUB_GET_NUKE");
                static uint8_t nuke_time_last = 20;
                if (nuke_time_last > 0)
                {
                    nuke_time_last--;
                    for (uint8_t i = 0; i < bomb_number; i++)
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
                else
                {
                    nuke_time_last = 0;
                }
            }
            break;
            }
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

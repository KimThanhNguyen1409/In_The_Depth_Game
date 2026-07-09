#include "itd_game_mainsub.h"

itd_game_mainsub_t mainsub;

bool itd_game_mainsub_check_hit_by_bomb(uint8_t bo)
{
    return ((((int16_t)bomb[bo].x) + BOMB_SIZE_BITMAP_X > mainsub.x + MAINSUB_HITBOX_LEFT_OFFSET) &&
            (((int16_t)bomb[bo].x) < mainsub.x + MAINSUB_HITBOX_RIGHT_OFFSET) &&
            (((int16_t)bomb[bo].y) + BOMB_SIZE_BITMAP_Y > mainsub.y + MAINSUB_HITBOX_TOP_OFFSET) &&
            (((int16_t)bomb[bo].y) < mainsub.y + MAINSUB_HITBOX_BOTTOM_OFFSET));
}
bool itd_game_mainsub_check_hit_by_spike(uint8_t sp)
{
    return (((int16_t)spike[sp].x + SPIKE_SINGLE_SIZE_BITMAP_X > mainsub.x + MAINSUB_HITBOX_LEFT_OFFSET) &&
            ((int16_t)spike[sp].x < mainsub.x + MAINSUB_HITBOX_RIGHT_OFFSET) &&
            ((int16_t)spike[sp].y + SPIKE_SINGLE_SIZE_BITMAP_Y > mainsub.y + MAINSUB_HITBOX_TOP_OFFSET) &&
            ((int16_t)spike[sp].y < mainsub.y + MAINSUB_HITBOX_BOTTOM_OFFSET));
}
void itd_game_mainsub_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case ITD_GAME_MAINSUB_SETUP:
        APP_DBG_SIG("ITD_GAME_MAINSUB_SETUP");
        mainsub.x = MAINSUB_AXIS_X;
        mainsub.y = MAINSUB_AXIS_Y;
        mainsub.visible = WHITE;
        mainsub.action_image = 1;
        break;
    case ITD_GAME_MAINSUB_GO_DOWN:
        APP_DBG_SIG("ITD_GAME_MAINSUB_GO_DOWN");
        if (mainsub.y < MAINSUB_AXIS_Y_MAX)
        {
            mainsub.y += MAINSUB_STEP_AXIS_Y;
        }
        else
        {
            mainsub.y = MAINSUB_AXIS_Y_MAX;
        }
        break;
    case ITD_GAME_MAINSUB_GO_UP:
        APP_DBG_SIG("ITD_GAME_MAINSUB_GO_UP");
        if (mainsub.y > MAINSUB_AXIS_Y_MIN)
        {
            mainsub.y -= MAINSUB_STEP_AXIS_Y;
        }
        else
        {
            mainsub.y = MAINSUB_AXIS_Y_MIN;
        }
        break;
    case ITD_GAME_MAINSUB_DENATOR_BY_BOMB:
        APP_DBG_SIG("ITD_GAME_MAINSUB_DENATOR_BY_BOMB");
        for (uint8_t i = 0; i < BOMB_NUMBER_MAX; i++)
        {
            if (bomb[i].visible != WHITE)
                continue;
            if (!itd_game_mainsub_check_hit_by_bomb(i))
                continue;
            bomb[i].visible = BLACK;
            bomb[i].x = 0;
            boom_spawn(mainsub.x, mainsub.y);
            mainsub.visible = BLACK;
        }
        break;
    case ITD_GAME_MAINSUB_DENATOR_BY_SPIKE:
        APP_DBG_SIG("ITD_GAME_MAINSUB_DENATOR_BY_SPIKE");
        for (uint8_t i = 0; i < SPIKE_NUMBER; i++)
        {
            if (spike[i].visible != WHITE)
                continue;
            if (!itd_game_mainsub_check_hit_by_spike(i))
                continue;
            spike[i].visible = BLACK;
            spike[i].x = 0;
            boom_spawn(mainsub.x, mainsub.y);
            mainsub.visible = BLACK;
        }
        break;
    case ITD_GAME_MAINSUB_RESET:
        APP_DBG_SIG("ITD_GAME_MAINSUB_RESET");
        mainsub.x = MAINSUB_AXIS_X;
        mainsub.y = MAINSUB_AXIS_Y;
        mainsub.visible = BLACK;
        break;
    default:
        break;
    }
}
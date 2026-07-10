#include "itd_game_heart.h"
uint8_t current_heart = 0;
itd_game_heart_t hearts[HEART_MAX_NUMBER];
static void itd_game_heart_reset_all()
{
    current_heart = 0;
    for (int i = 0; i < HEART_MAX_NUMBER; i++)
    {
        hearts[i].x = 0;
        hearts[i].y = 0;
        hearts[i].visible = BLACK;
    }
}
void itd_game_heart_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case ITD_GAME_HEART_SETUP:
        APP_DBG_SIG("ITD_GAME_HEART_SETUP");
        current_heart = HEART_MAX_NUMBER;
        for (uint8_t i = 0; i < current_heart; i++)
        {
            hearts[i].x += HEART_AXIS_X + i * HEART_DISTANCE_AXIS_X;
            hearts[i].y = HEART_AXIS_Y;
            hearts[i].visible = WHITE;
        }
        break;
    case ITD_GAME_HEART_UPDATE:
        APP_DBG_SIG("ITD_GAME_HEAR_UPDATE");
        for (uint8_t i = 0; i < BOMB_NUMBER_MAX; i++)
        {
            if (itd_game_mainsub_check_hit_by_bomb(i))
            {
                hearts[current_heart].visible = BLACK;
            }
        }
        for (uint8_t i = 0; i < SPIKE_NUMBER; i++)
        {
            if (itd_game_mainsub_check_hit_by_spike(i, spikes[i].type))
            {
                hearts[current_heart].visible = BLACK;
            }
        }
        break;
    case ITD_GAME_HEART_RESET:
        APP_DBG_SIG("ITD_GAME_HEART_RESET");
        itd_game_heart_reset_all();
        break;
    default:
        break;
    }
}
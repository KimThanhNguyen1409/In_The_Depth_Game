#include "itd_game_heart.h"

itd_game_heart_t heart[HEART_MAX_NUMBER];
static void itd_game_heart_reset_all()
{
    for (int i = 0; i < HEART_MAX_NUMBER; i++)
    {
        heart[i].x = 0;
        heart[i].y = 0;
        heart[i].visible = BLACK;
    }
}
void itd_game_heart_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case ITD_GAME_HEART_SETUP:
        APP_DBG_SIG("ITD_GAME_HEART_SETUP");
        for (int i = 0; i < HEART_MAX_NUMBER; i++)
        {
            heart[i].x += HEART_AXIS_X + i * HEART_DISTANCE_AXIS_X;
            heart[i].y = HEART_AXIS_Y;
            heart[i].visible = WHITE;
        }
        break;
    case ITD_GAME_HEART_UPDATE:
        break;
    case ITD_GAME_HEART_RESET:
        APP_DBG_SIG("ITD_GAME_HEART_RESET");
        itd_game_heart_reset_all();
        break;
    default:
        break;
    }
}
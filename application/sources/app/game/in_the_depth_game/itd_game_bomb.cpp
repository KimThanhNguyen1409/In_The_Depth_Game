#include "itd_game_bomb.h"

itd_game_bomb_t bomb[BOMB_NUMBER_MAX];
static void itd_game_bomb_reset_all()
{
    for (int i = 0; i < BOMB_NUMBER_MAX; i++)
    {
        bomb[i].x = 0;
        bomb[i].y = 0;
        bomb[i].visible = BLACK;
    }
}
void itd_game_bomb_handle(ak_msg_t *msg)
{
    int spawn_number = BOMB_INITAL_NUMBER + rand() % (BOMB_NUMBER_MAX - BOMB_INITAL_NUMBER + 1);
    switch (msg->sig)
    {
    case ITD_GAME_BOMB_SETUP:
        APP_DBG_SIG("ITD_GAME_BOMB_SETUP");
        itd_game_bomb_reset_all();
        break;
    case ITD_GAME_BOMB_SPAWN:
        APP_DBG_SIG("ITD_GAME_BOMB_SPAWN");
        for (int i = 0; i < spawn_number; i++)
        {
            bomb[i].x = BOMB_SPAWN_AXIS_X_MIN + rand() % (BOMB_SPAWN_AXIS_X_MAX - BOMB_SPAWN_AXIS_X_MIN + 1);
            bomb[i].y = BOMB_SPAWN_AXIS_Y_MIN + rand() % (BOMB_SPAWN_AXIS_Y_MAX - BOMB_SPAWN_AXIS_Y_MIN + 1);
            bomb[i].visible = WHITE;
        }
        break;
    case ITD_GAME_BOMB_GO:
        APP_DBG_SIG("ITD_GAME_BOMB_GO");
        for (int i = 0; i < spawn_number; i++)
        {
            if (bomb[i].visible != BLACK)
                continue;
            bomb[i].x -= BOMB_STEP_AXIS_X;
            if (bomb[i].x <= BOMB_DESPAWN_AXIS_X)
            {
                bomb[i].visible = BLACK;
                bomb[i].x = 0;
            }
        }
        break;
    case ITD_GAME_BOMB_RESET:
        APP_DBG_SIG("ITD_GAME_BOMB_RESET");
        itd_game_bomb_reset_all();
    default:
        break;
    }
}

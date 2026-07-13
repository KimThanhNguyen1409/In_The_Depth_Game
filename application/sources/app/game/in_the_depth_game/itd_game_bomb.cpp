#include "itd_game_bomb.h"

itd_game_bomb_t bombs[BOMB_NUMBER_MAX];
uint8_t bomb_number = BOMB_INITAL_NUMBER + rand() % (BOMB_NUMBER_MAX - BOMB_INITAL_NUMBER + 1);
static void itd_game_bomb_reset_all()
{
    for (int i = 0; i < BOMB_NUMBER_MAX; i++)
    {
        bombs[i].x = 0;
        bombs[i].y = 0;
        bombs[i].visible = BLACK;
    }
}
void itd_game_bomb_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case ITD_GAME_BOMB_SETUP:
    {
        APP_DBG_SIG("ITD_GAME_BOMB_SETUP");
        itd_game_bomb_reset_all();
    }
    break;
    case ITD_GAME_BOMB_SPAWN:
    {
        APP_DBG_SIG("ITD_GAME_BOMB_SPAWN");
        for (int i = 0; i < bomb_number; i++)
        {
            bombs[i].x = BOMB_SPAWN_AXIS_X_MIN + rand() % (BOMB_SPAWN_AXIS_X_MAX - BOMB_SPAWN_AXIS_X_MIN + 1);
            bombs[i].y = BOMB_SPAWN_AXIS_Y_MIN + rand() % (BOMB_SPAWN_AXIS_Y_MAX - BOMB_SPAWN_AXIS_Y_MIN + 1);
            bombs[i].visible = WHITE;
            break;
        }
    }
    break;
    case ITD_GAME_BOMB_GO:
    {
        APP_DBG_SIG("ITD_GAME_BOMB_GO");
        for (int i = 0; i < bomb_number; i++)
        {
            if (bombs[i].visible != BLACK)
                continue;
            bombs[i].x -= BOMB_STEP_AXIS_X;
            if (bombs[i].x <= BOMB_DESPAWN_AXIS_X)
            {
                bombs[i].visible = BLACK;
                bombs[i].x = 0;
            }
        }
    }
    break;
    case ITD_GAME_BOMB_RESET:
    {
        APP_DBG_SIG("ITD_GAME_BOMB_RESET");
        itd_game_bomb_reset_all();
    }
    break;
    default:
        break;
    }
}

#include "itd_game_coin.h"

itd_game_coin_t coin[COIN_NUMBER_MAX];
uint8_t coin_number = COIN_INITAL_NUMBER + rand() % (COIN_NUMBER_MAX - COIN_INITAL_NUMBER);

static void itd_game_coin_reset_all()
{
    current_coin = 0;
    for (int i = 0; i < COIN_NUMBER_MAX; i++)
    {
        coin[i].x = 0;
        coin[i].y = 0;
        coin[i].visible = BLACK;
    }
}

void itd_game_coin_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case ITD_GAME_COIN_SETUP:
        APP_DBG_SIG("ITD_GAME_COIN_SETUP");
        itd_game_coin_reset_all();
        break;
    case ITD_GAME_COIN_SPAWN:
        APP_DBG_SIG("ITD_GAME_COIN_SPAWN");
        for (int i = 0; i < coin_number; i++)
        {
            coin[i].x = COIN_SPAWN_AXIS_X_MIN + rand() % (COIN_SPAWN_AXIS_X_MAX - COIN_SPAWN_AXIS_X_MIN);
            coin[i].y = COIN_SPAWN_AXIS_Y_MIN + rand() % (COIN_SPAWN_AXIS_Y_MAX - COIN_SPAWN_AXIS_Y_MIN);
            coin[i].visible = WHITE;
        }
        break;
    case ITD_GAME_COIN_GO:
        APP_DBG_SIG("ITD_GAME_COIN_GO");
        for (int i = 0; i < coin_number; i++)
        {
            if (coin[i].visible != WHITE)
                continue;
            if (coin[i].x <= COIN_DESPAWN_AXIS_X)
            {
                coin[i].x = 0;
                coin[i].visible = BLACK;
            }
        }
        break;
    case ITD_GAME_COIN_RESET:
        APP_DBG_SIG("ITD_GAME_COIN_RESET");
        itd_game_coin_reset_all();
        break;
    default:
        break;
    }
}
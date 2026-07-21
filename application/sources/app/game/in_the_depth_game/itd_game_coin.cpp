#include "itd_game_coin.h"

itd_game_coin_t coins[COIN_NUMBER_MAX];
uint8_t coin_number = COIN_INITAL_NUMBER + rand() % (COIN_NUMBER_MAX - COIN_INITAL_NUMBER);
uint8_t current_coin = 0;
static void itd_game_coin_reset_all()
{
    current_coin = 0;
    for (int i = 0; i < COIN_NUMBER_MAX; i++)
    {
        coins[i].x = 0;
        coins[i].y = 0;
        coins[i].visible = BLACK;
    }
}

void itd_game_coin_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case ITD_GAME_COIN_SETUP:
    {
        APP_DBG_SIG("ITD_GAME_COIN_SETUP");
        itd_game_coin_reset_all();
    }
    break;
    case ITD_GAME_COIN_SPAWN:
    {
        APP_DBG_SIG("ITD_GAME_COIN_SPAWN");
        static uint8_t coin_cooldown = 0;
        if(coin_cooldown > 0){
            coin_cooldown--; 
        }
        else
        {
            for (int i = 0; i < coin_number; i++)
            {
                if (coins[i].visible == WHITE) 
                    continue;
                coins[i].x = COIN_SPAWN_AXIS_X_MIN + rand() % (COIN_SPAWN_AXIS_X_MAX - COIN_SPAWN_AXIS_X_MIN) + COIN_SPAWN_LEFT_OFFSET;
                coins[i].y = COIN_SPAWN_AXIS_Y_MIN + rand() % (COIN_SPAWN_AXIS_Y_MAX - COIN_SPAWN_AXIS_Y_MIN) + COIN_SPAWN_TOP_OFFSET;
                coins[i].visible = WHITE;
                coin_cooldown = 8 + rand() % 8;
                break;
            }
        }
        for (int i = 0; i < coin_number; i++)
        {
            if (coins[i].visible != WHITE)
                continue;
            if (coins[i].x <= COIN_DESPAWN_AXIS_X + COIN_STEP_AXIS_X)
            {
                coins[i].x = 0;
                coins[i].visible = BLACK;
            }
            else{
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
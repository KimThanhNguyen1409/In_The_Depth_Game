#include "itd_game_gift.h"

itd_game_gift_t gifts[GIFT_NUMBER_MAX];
uint8_t gift_number = 1 + rand() % (GIFT_NUMBER_MAX - 1);
buff_type_t current_options[3];

void itd_game_generate_buff_options()
{
    uint8_t count = 0;
    while (count < 3)
    {
        buff_type_t rand_buff = (buff_type_t)(rand() % 4);
        for (uint8_t i = 0; i < count; i++)
        {
            if (current_options[i] == rand_buff)
            {
                break;
            }
        }
        current_options[count] = rand_buff;
        count++;    
    }
}

void itd_game_gift_restet_all()
{
    for (int i = 0; i < gift_number; i++)
    {
        gifts[i].x = 0;
        gifts[i].y = 0;
        gifts[i].visible = BLACK;
    }
}

void itd_game_gift_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case ITD_GAME_GIFT_SETUP:
    {
        APP_DBG_SIG("ITD_GAME_GIFT_SETUP");
        itd_game_gift_restet_all();
    }
    break;
    case ITD_GAME_GIFT_SPAWN:
    {
        APP_DBG_SIG("ITD_GAME_GIFT_SPAWN");
        for (uint8_t i = 0; i < gift_number; i++)
        {
            gifts[i].x = GIFT_SPAWN_AXIS_X_MIN + rand() % (GIFT_SPAWN_AXIS_X_MAX - GIFT_SPAWN_AXIS_X_MIN);
            gifts[i].y = GIFT_SPAWN_AXIS_Y_MIN + rand() % (GIFT_SPAWN_AXIS_Y_MAX - GIFT_SPAWN_AXIS_Y_MIN);
            gifts[i].visible = WHITE;
        }
    }
    break;
    case ITD_GAME_GIFT_GO:
    {
        APP_DBG_SIG("ITD_GAME_GIFT_GO");
        for (uint8_t i = 0; i < gift_number; i++)
        {
            if (gifts[i].visible == WHITE)
                continue;
            gifts[i].x -= GIFT_STEP_AXIS_X;
            if (gifts[i].x <= GIFT_DESPAWN_AXIS_X)
            {
                gifts[i].visible = BLACK;
                gifts[i].x = 0;
            }
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
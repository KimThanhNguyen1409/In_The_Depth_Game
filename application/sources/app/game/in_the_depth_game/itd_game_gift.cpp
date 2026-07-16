#include "itd_game_gift.h"

itd_game_gift_t gifts[GIFT_NUMBER_MAX];
uint8_t gift_number = GIFT_INITAL_NUMBER + rand() % (GIFT_NUMBER_MAX - GIFT_INITAL_NUMBER + 1);

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
        static uint8_t gift_cooldown = 0;
        if(gift_cooldown > 0)
            gift_cooldown--;
        for (uint8_t i = 0; i < gift_number; i++)
        {
            if(gifts[i].visible == WHITE)
                continue;
            gifts[i].x = GIFT_SPAWN_AXIS_X_MIN + rand() % (GIFT_SPAWN_AXIS_X_MAX - GIFT_SPAWN_AXIS_X_MIN) + GIFT_SPAWN_LEFT_OFFSET;
            gifts[i].y = GIFT_SPAWN_AXIS_Y_MIN + rand() % (GIFT_SPAWN_AXIS_Y_MAX - GIFT_SPAWN_AXIS_Y_MIN) + GIFT_SPAWN_TOP_OFFSET;
            gifts[i].visible = WHITE;
            gifts[i].buff = (buff_type_t)(rand() % 4);
            gift_cooldown = 6 + rand() % 6;
        }
    }
    break;
    case ITD_GAME_GIFT_GO:
    {
        APP_DBG_SIG("ITD_GAME_GIFT_GO");
        for (uint8_t i = 0; i < gift_number; i++)
        {
            if (gifts[i].visible != WHITE)
                continue;
            if(gifts[i].x <= GIFT_DESPAWN_AXIS_X + GIFT_STEP_AXIS_X){
                gifts[i].x = 0;
                gifts[i].visible = BLACK;
            }else{
                gifts[i].x -= GIFT_STEP_AXIS_X;
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
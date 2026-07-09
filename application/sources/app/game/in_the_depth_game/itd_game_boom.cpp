#include "itd_game_boom.h"

itd_game_boom_t boom[BOOM_NUMBER];

static void itd_game_boom_reset_all()
{
    for (int i = 0; i < BOOM_NUMBER; i++)
    {
        boom[i].x = 0;
        boom[i].y = 0;
        boom[i].visible = BLACK;
    }
}
void itd_game_boom_spawn(uint16_t x, uint16_t y)
{
    for (uint8_t i = 0; i < BOOM_NUMBER; i++)
    {
        if (boom[i].visible == WHITE)
            continue;
        boom[i].visible = WHITE;
        boom[i].x = (x + BOOM_SPAWN_OFFSET_X > 0) ? (uint8_t)(x + BOOM_SPAWN_OFFSET_X) : 0;
        boom[i].y = (y >= BOOM_SPAWN_OFFSET_Y) ? y - BOOM_SPAWN_OFFSET_Y : 0;
        boom[i].action_image = 1;
        break;
    }
}
void itd_game_boom_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case ITD_GAME_BOOM_SETUP:
        APP_DBG_SIG("ITD_GAME_BOOM_SETUP");
        itd_game_boom_reset_all();
        break;
    case ITD_GAME_BOOM_UPDATE:
        APP_DBG_SIG("ITD_GAME_BOOM_UPDATE");
        for (uint8_t i = 0; i < BOOM_NUMBER; i++)
        {
            if (boom[i].visible == WHITE)
            {
                if (boom[i].action_image >= 6)
                {
                    boom[i].action_image = 1;
                    boom[i].visible == BLACK;
                }
                else
                {
                    boom[i].action_image++;
                }
            }
        }
        break;
    case ITD_GAME_BOOM_RESET:
        APP_DBG_SIG("ITD_GAME_BOOM_RESET");
        itd_game_boom_reset_all();
        break;
    default:
        break;
    }
}
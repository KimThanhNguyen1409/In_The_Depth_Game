#include "itd_game_spike.h"

itd_game_spike_t spike[SPIKE_NUMBER];
static void itd_game_spike_reset_all()
{
    for (int i = 0; i < SPIKE_NUMBER; i++)
    {
        spike[i].x = 0;
        spike[i].type = 0;
        spike[i].visible = BLACK;
    }
}
void itd_game_spike_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case ITD_GAME_SPIKE_SETUP:
        APP_DBG_SIG("ITD_GAME_SPIKE_SETUP");
        itd_game_spike_reset_all();
        break;
    case ITD_GAME_SPIKE_SPAWN:
        APP_DBG_SIG("ITD_GAME_SPIKE_SPAWN");
        for (int i = 0; i < SPIKE_NUMBER; i++)
        {
            int type = 2 + rand() % 1;
            if (type == 3)
            {
                spike[i].type = SPIKE_TRIPLE_TYPE;
                spike[i].x = SPIKE_TRIPLE_SPAWN_X;
                spike[i].visible = WHITE;
            }
            else if (type == 2)
            {
                spike[i].type = SPIKE_SINGLE_TYPE;
                spike[i].x = SPIKE_SINGLE_SPAWN_X;
                spike[i].visible = WHITE;
            }
        }
        break;
    case ITD_GAME_SPIKE_GO:
        for (int i = 0; i < SPIKE_NUMBER; i++)
        {
            if (spike[i].visible != WHITE)
                continue;
            spike[i].x -= SPIKE_STEP_X;
            if (spike[i].x <= 10)
            {
                spike[i].visible = BLACK;
                spike[i].x = 0;
            }
        }
        break;
    case ITD_GAME_SPIKE_RESET:
        APP_DBG_SIG("ITD_GAME_SPIKE_RESET");
        itd_game_spike_reset_all();
        break;
    default:
        break;
    }
}
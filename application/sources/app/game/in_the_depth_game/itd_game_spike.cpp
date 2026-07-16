#include "itd_game_spike.h"

itd_game_spike_t spikes[SPIKE_NUMBER];
static void itd_game_spike_reset_all()
{
    for (int i = 0; i < SPIKE_NUMBER; i++)
    {
        spikes[i].x = 0;
        spikes[i].y = 0;
        spikes[i].type = 0;
        spikes[i].visible = BLACK;
    }
}
void itd_game_spike_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case ITD_GAME_SPIKE_SETUP:
    {
        APP_DBG_SIG("ITD_GAME_SPIKE_SETUP");
        itd_game_spike_reset_all();
    }
    break;
    case ITD_GAME_SPIKE_SPAWN:
    {
        APP_DBG_SIG("ITD_GAME_SPIKE_SPAWN");
        for (int i = 0; i < SPIKE_NUMBER; i++)
        {
            if(spikes[i].visible == WHITE)
                continue;
            int type = 2 + rand() % 2;
            if (type == 3)
            {
                spikes[i].type = SPIKE_TRIPLE_TYPE;
                spikes[i].x = SPIKE_TRIPLE_SPAWN_X - SPIKE_SPAWN_OFFSET;
                spikes[i].y = SPIKE_TRIPLE_SPAWN_Y; 
                spikes[i].visible = WHITE;
            }
            else if (type == 2)
            {
                spikes[i].type = SPIKE_SINGLE_TYPE;
                spikes[i].x = SPIKE_SINGLE_SPAWN_X - SPIKE_SPAWN_OFFSET;
                spikes[i].y = SPIKE_SINGLE_SPAWN_Y;
                spikes[i].visible = WHITE;
            }
        }
    }
    break;
    case ITD_GAME_SPIKE_GO:
    {
        APP_DBG_SIG("ITD_GAME_SPIKE_GO");
        for (int i = 0; i < SPIKE_NUMBER; i++)
        {
            if (spikes[i].visible != WHITE)
                continue;
            spikes[i].x -= SPIKE_STEP_X;
            if (spikes[i].x <= SPIKE_DESPAWN_AXIS_X)
            {
                spikes[i].visible = BLACK;
                spikes[i].x = 0;
            }
        }
    }
    break;
    case ITD_GAME_SPIKE_RESET:
    {
        APP_DBG_SIG("ITD_GAME_SPIKE_RESET");
        itd_game_spike_reset_all();
    }
    break;
    default:
        break;
    }
}
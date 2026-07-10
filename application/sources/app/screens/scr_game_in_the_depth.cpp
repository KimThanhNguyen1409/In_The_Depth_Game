#include "scr_game_in_the_depth.h"

#define MAINSUB_NONE (0)
#define MAINSUB_UP (1)
#define MAINSUB_DOWN (2)

static uint8_t itd_game_state;
static uint8_t mainsub_dir = MAINSUB_NONE;

void itd_game_frame_display()
{
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(32, 8);
    view_render.print("SCORE:");
    view_render.print(itd_game_score);
    view_render.setCursor(80, 8);
    view_render.print("TIME:");
    view_render.print(itd_game_time);
    view_render.drawLine(0, 16, 128, 16, WHITE);
}

static void view_scr_game_in_th_depth();

view_dynamic_t dyn_view_in_the_depth = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game_in_th_depth};

view_screen_t scr_game_in_the_depth = {
    &dyn_view_in_the_depth,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

void itd_game_mainsub_display()
{
    if (mainsub.visible != WHITE)
        return;
    const unsigned char *frame = main_sub;
    view_render.drawBitmap(16, 40, frame, MAINSUB_SIZE_BITMAP_X, MAINSUB_SIZE_BITMAP_Y, WHITE);
}

void itd_game_bomb_display()
{
    for (uint8_t i = 0; i < bomb_number; i++)
    {
        if (bombs[i].visible != WHITE)
            continue;
        view_render.drawBitmap(bombs[i].x, bombs[i].y, bomb, BOMB_SIZE_BITMAP_X, BOMB_SIZE_BITMAP_Y, WHITE);
    }
}

void itd_game_spike_display()
{
    for (uint8_t i = 0; i < SPIKE_NUMBER; i++)
    {
        if (spikes[i].type == 2)
        {
            if (spikes[i].visible != WHITE)
                continue;
            view_render.drawBitmap(spikes[i].x, spikes[i].y, spike_single, SPIKE_SINGLE_SIZE_BITMAP_X, SPIKE_SINGLE_SIZE_BITMAP_Y, WHITE);
        }
        else
        {
            if (spikes[i].visible != WHITE)
                continue;
            view_render.drawBitmap(spikes[i].x, spikes[i].y, spike_triple, SPIKE_TRIPLE_SIZE_BITMAP_X, SPIKE_TRIPLE_SIZE_BITMAP_Y, WHITE);
        }
    }
}
void itd_game_coin_display()
{
    for (uint8_t i = 0; i < coin_number; i++)
    {
        if (coins[i].visible != WHITE)
            continue;
        view_render.drawBitmap(coins[i].x, coins[i].y, coin, COIN_SIZE_BITMAP_X, COIN_SIZE_BITMAP_Y, WHITE);
    }
}
void itd_game_boom_display()
{
    for (uint8_t i = 0; i < BOOM_NUMBER; i++)
    {
        if (boom[i].visible != WHITE)
            continue;
        const unsigned char *frame = boom_I;
        uint8_t w = BOOM_BITMAP_SIZE_X;
        uint8_t h = BOOM_BITMAP_SIZE_Y;
        if (boom[i].action_image == 2)
        {
            frame = boom_II;
        }
        else if (boom[i].action_image == 3)
        {
            frame = boom_III;
        }
        else if (boom[i].action_image == 4)
        {
            frame = boom_IV;
        }
        else if (boom[i].action_image == 5)
        {
            frame = boom_V;
        }
        else if (boom[i].action_image == 6)
        {
            frame = boom_VI;
        }
        view_render.drawBitmap(boom[i].x, boom[i].y, frame, w, h, WHITE);
    }
}
void itd_game_heart_display()
{
    for (uint8_t i = 0; i < current_heart; i++)
    {
        if (hearts[i].visible != WHITE)
            continue;
        view_render.drawBitmap(hearts[i].x, hearts[i].y, heart, HEART_SIZE_BITMAP_X, HEART_SIZE_BITMAP_Y, WHITE);
    }
}

#include "scr_game_in_the_depth.h"

#define MAINSUB_NONE (0)
#define MAINSUB_UP (1)
#define MAINSUB_DOWN (2)

static uint8_t mainsub_dir = MAINSUB_NONE;
static uint8_t itd_game_state = GAME_OVER;
static uint8_t grid_offset_x = 0;

itd_game_setting_t settingsetup;

void itd_game_frame_display()
{
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(14, 4);
    view_render.print("X");
    view_render.print(itd_game_heart);
    view_render.drawBitmap(26, 2, shield, SHIELD_BITMAP_SIZE_X, SHIELD_BITMAP_SIZE_Y, WHITE);
    view_render.setCursor(38, 4);
    view_render.print("X");
    view_render.print(itd_game_shield);
    view_render.setCursor(54, 4);
    view_render.print("S:");
    view_render.print(itd_game_score);
    view_render.setCursor(92, 4);
    view_render.print("T:");    
    view_render.print(itd_game_time / 10);
    view_render.drawLine(0, 15, 128, 15, WHITE);
    view_render.drawBitmap(0, 54, seabottom, SEABOTTOM_BITMAP_AXIS_X, SEABOTTOM_BITMAP_AXIS_Y, WHITE);
}
void itd_game_particle_display(){
    for(uint8_t i = 0; i <= 128; i += 8){
        view_render.drawPixel(i - grid_offset_x, 18, WHITE);
        view_render.drawPixel(i - grid_offset_x, 28, WHITE);
        view_render.drawPixel(i - grid_offset_x, 38, WHITE);
        view_render.drawPixel(i - grid_offset_x, 48, WHITE);
    }
}
static void view_scr_game_in_the_depth();

view_dynamic_t dyn_view_in_the_depth = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game_in_the_depth};

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
    view_render.drawBitmap(mainsub.x, mainsub.y, main_sub, MAINSUB_SIZE_BITMAP_X, MAINSUB_SIZE_BITMAP_Y, WHITE);
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
            view_render.drawBitmap(spikes[i].x, spikes[i].y, spike_tall, SPIKE_TALL_SIZE_BITMAP_X, SPIKE_TALL_SIZE_BITMAP_Y, WHITE);
               
        }
        else
        {
            if (spikes[i].visible != WHITE)
                continue;
             view_render.drawBitmap(spikes[i].x, spikes[i].y, spike_short, SPIKE_SHORT_SIZE_BITMAP_X, SPIKE_SHORT_SIZE_BITMAP_Y, WHITE);
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
    for (uint8_t i = 0; i < current_heart && i < HEART_MAX_NUMBER; i++)
    {
        if (hearts[i].visible != WHITE)
            continue;
        view_render.drawBitmap(hearts[i].x, hearts[i].y, heart, HEART_SIZE_BITMAP_X, HEART_SIZE_BITMAP_Y, WHITE);
    }
}
void itd_game_gift_display()
{
    for (uint8_t i = 0; i < gift_number; i++)
    {
        if (gifts[i].visible != WHITE)
            continue;
        view_render.drawBitmap(gifts[i].x, gifts[i].y, gift, GIFT_SIZE_BITMAP_X, GIFT_SIZE_BITMAP_Y, WHITE);
    }
}
void view_scr_game_in_the_depth()
{
    if (itd_game_state == GAME_PLAY)
    {
        itd_game_frame_display();
        itd_game_particle_display();
        itd_game_mainsub_display();
        itd_game_bomb_display();
        itd_game_boom_display();
        itd_game_coin_display();
        itd_game_heart_display();
        itd_game_spike_display();
        itd_game_gift_display();
    }
    else if (itd_game_state == GAME_OVER)
    {
        view_render.setTextColor(1);
        view_render.setTextColor(WHITE);
        view_render.clear();
        view_render.drawBitmap(32, 0, sub_sinking, 64, 64, WHITE);
        view_render.drawBitmap(0, 58, seabottom, SEABOTTOM_BITMAP_AXIS_X, SEABOTTOM_BITMAP_AXIS_Y, WHITE);
        // view_render.drawBitmap(38, 0, sink_letter, 64, 7, WHITE);
        view_render.setCursor(38, 0);
        view_render.print("YOU SINK");
        BUZZER_PlaySound(BUZZER_SOUND_GOODBYE);
    }
}
void scr_game_in_the_depth_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SCREEN_ENTRY:
    {
        APP_DBG_SIG("ITD_GAME SCREEN_ENTRY\n");
        itd_game_setting_read(&settings);
        BUZZER_Silent(settings.sound ? BUZZER_SILENT_OFF : BUZZER_SILENT_ON); 
        task_post_pure_msg(ITD_GAME_MAINSUB_ID, ITD_GAME_MAINSUB_SETUP);
        task_post_pure_msg(ITD_GAME_BOMB_ID, ITD_GAME_BOMB_SETUP);
        task_post_pure_msg(ITD_GAME_BOOM_ID, ITD_GAME_BOOM_SETUP);
        task_post_pure_msg(ITD_GAME_COIN_ID, ITD_GAME_COIN_SETUP);
        task_post_pure_msg(ITD_GAME_HEART_ID, ITD_GAME_HEART_SETUP);
        task_post_pure_msg(ITD_GAME_BORDER_ID, ITD_GAME_BORDER_SETUP);
        task_post_pure_msg(ITD_GAME_SPIKE_ID, ITD_GAME_SPIKE_SETUP);
        task_post_pure_msg(ITD_GAME_GIFT_ID, ITD_GAME_GIFT_SETUP);
        itd_game_state = GAME_PLAY;
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
        timer_set(AC_TASK_DISPLAY_ID,
                  ITD_GAME_TIME_TICK,
                  ITD_GAME_TIME_TICK_INTERVAL,
                  TIMER_PERIODIC);
    }
    break;
    case ITD_GAME_TIME_TICK:
    {
        APP_DBG_SIG("ITD_GAME_TIME_TICK\n");
        if (itd_game_state != GAME_PLAY)
            break;
        grid_offset_x++;
        if(grid_offset_x >= 8)
            grid_offset_x = 0;
        if(mainsub_dir == MAINSUB_DOWN)
        {
            task_post_pure_msg(ITD_GAME_MAINSUB_ID, ITD_GAME_MAINSUB_GO_DOWN);
        }
        else if(mainsub_dir == MAINSUB_UP)
        {
            task_post_pure_msg(ITD_GAME_MAINSUB_ID, ITD_GAME_MAINSUB_GO_UP);
        }
        task_post_pure_msg(ITD_GAME_BOMB_ID, ITD_GAME_BOMB_SPAWN);
        task_post_pure_msg(ITD_GAME_SPIKE_ID, ITD_GAME_SPIKE_SPAWN);
        task_post_pure_msg(ITD_GAME_COIN_ID, ITD_GAME_COIN_SPAWN);
        task_post_pure_msg(ITD_GAME_GIFT_ID, ITD_GAME_GIFT_SPAWN);
        task_post_pure_msg(ITD_GAME_HEART_ID, ITD_GAME_HEART_UPDATE);
        task_post_pure_msg(ITD_GAME_MAINSUB_ID, ITD_GAME_MAINSUB_UPDATE);
        task_post_pure_msg(ITD_GAME_BOOM_ID, ITD_GAME_BOOM_UPDATE);
        task_post_pure_msg(ITD_GAME_BORDER_ID, ITD_GAME_BORDER_CHECK_GAME_OVER);
        task_post_pure_msg(ITD_GAME_BORDER_ID, ITD_GAME_BORDER_UPDATE);
    }
    break;
    case ITD_GAME_RESET:
    {
        APP_DBG_SIG("ITD_GAME_RESET\n");
        if(itd_game_heart == 0){
            if (itd_game_state != GAME_PLAY)
                break;
            scores.score_now = itd_game_score;
            time_last = itd_game_time;
            timer_remove_attr(AC_TASK_DISPLAY_ID, ITD_GAME_TIME_TICK);
            task_post_pure_msg(ITD_GAME_MAINSUB_ID, ITD_GAME_MAINSUB_RESET);
            task_post_pure_msg(ITD_GAME_BOMB_ID, ITD_GAME_BOMB_RESET);
            task_post_pure_msg(ITD_GAME_SPIKE_ID, ITD_GAME_SPIKE_RESET);
            task_post_pure_msg(ITD_GAME_COIN_ID, ITD_GAME_COIN_RESET);
            task_post_pure_msg(ITD_GAME_HEART_ID, ITD_GAME_HEART_RESET);
            task_post_pure_msg(ITD_GAME_BORDER_ID, ITD_GAME_BORDER_RESET);
            task_post_pure_msg(ITD_GAME_BOOM_ID, ITD_GAME_BOOM_RESET);
            task_post_pure_msg(ITD_GAME_GIFT_ID, ITD_GAME_GIFT_RESET);
            itd_game_state = GAME_OVER;
            timer_set(AC_TASK_DISPLAY_ID,
                    ITD_GAME_EXIT_GAME,
                    ITD_GAME_TIME_EXIT_INTERVAL,
                    TIMER_ONE_SHOT);            
        }
    }
    break;
    case AC_DISPLAY_BUTTON_UP_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTN_UP_PRESSED");
        mainsub_dir = MAINSUB_UP;
    }
    break;
    case AC_DISPLAY_BUTTON_UP_RELEASE:
    {
        APP_DBG_SIG("AC_DISPLAY_BTN_UP_RELEASE");
        if(mainsub_dir == MAINSUB_UP)
            mainsub_dir = MAINSUB_NONE;
    }
    break;
    case AC_DISPLAY_BUTTON_DOWN_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTN_DOWN_PRESSED");
        mainsub_dir = MAINSUB_DOWN;
    }
    break;
    case AC_DISPLAY_BUTTON_DOWN_RELEASE:
    {
        APP_DBG_SIG("AC_DISPLAY_BTN_DOWN_RELEASE");
        if(mainsub_dir == MAINSUB_DOWN)
            mainsub_dir = MAINSUB_NONE;
    }
    break;
    case ITD_GAME_EXIT_GAME:
    {
        APP_DBG_SIG("ITD_GAME_EXIT_GAME\n");
        itd_game_state = GAME_OVER;
        SCREEN_TRAN(scr_game_over_handle, &scr_game_over);
    }
    break;
    default:
        break;
    }
}
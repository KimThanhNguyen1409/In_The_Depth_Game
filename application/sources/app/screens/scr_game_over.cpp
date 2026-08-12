#include "scr_game_over.h"

static void view_scr_game_over();
view_dynamic_t dyn_view_game_over = {
    {
     .item_type = ITEM_TYPE_DYNAMIC,
     },
    view_scr_game_over,
};

view_screen_t scr_game_over = {
    &dyn_view_game_over,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

/*-----------------------------Game over Display-----------------------------*/
void view_scr_game_over()
{
    static uint32_t bub_cooldown = 0;
    bub_cooldown++;
    view_render.clear();
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t bub_x = 15 + i * 30;
        uint8_t bub_y = 64 - ((bub_cooldown * i + 2 * 15) % 64);
        view_render.drawCircle(bub_x, bub_y, 1 + (i % 2), WHITE);
    }
    view_render.drawBitmap(4, 1, gameover_sub, 120, 40, WHITE);
    view_render.setCursor(29, 18);
    view_render.print("SCORE");
    view_render.setCursor(75, 18);
    view_render.print(scores.score_now);
    view_render.setCursor(29, 26);
    view_render.print("TIME");
    view_render.setCursor(75, 26);
    view_render.print(time_last / 10);
    const uint8_t btn_cx[3]     = {26, 64, 102};
    const uint8_t btn_cy        = 51;
    const uint8_t btn_r         = 12;
    const uint8_t* btn_icons[3] = {retry, rank, home};
    const uint8_t btn_w[3]      = {20, 20, 22};
    const uint8_t btn_h[3]      = {22, 22, 22};
    for (uint8_t i = 0; i < 3; i++)
    {
        view_render.fillCircle(btn_cx[i], btn_cy, btn_r, BLACK);
        view_render.drawCircle(btn_cx[i], btn_cy, btn_r, WHITE);
        uint8_t imgx = btn_cx[i] - (btn_w[i] / 2);
        uint8_t imgy = btn_cy - (btn_h[i] / 2);
        view_render.drawBitmap(imgx, imgy, btn_icons[i], btn_w[i], btn_h[i], WHITE);
    }
}
/*-----------------------------Game Ranking-----------------------------*/
void game_ranking()
{
    if (scores.score_now > scores.score_1st)
    {
        scores.score_3rd = scores.score_2st;
        scores.score_2st = scores.score_1st;
        scores.score_1st = scores.score_now;
    }
    else if (scores.score_now > scores.score_2st)
    {
        scores.score_3rd = scores.score_2st;
        scores.score_2st = scores.score_now;
    }
    else
    {
        scores.score_3rd = scores.score_now;
    }
}
/*-----------------------------Game over handle-----------------------------*/
void scr_game_over_handle(ak_msg_t* msg)
{
    switch (msg->sig)
    {
        case SCREEN_ENTRY:
        {
            APP_DBG_SIG("SCREEN_ENTRY\n");
            view_render.initialize();
            view_render_display_on();
            itd_game_setting_read(&settings);
            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_ANIMATION_TICK, 100, TIMER_PERIODIC);
            game_ranking();
            BUZZER_PlaySound(BUZZER_SOUND_GAME_OVER);
        }
        break;
        case AC_DISPLAY_BUTTON_MODE_PRESSED:
        {
            APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_PRESSED");
            itd_game_scores_write(&scores);
            SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
        }
        break;
        case AC_DISPLAY_BUTTON_DOWN_PRESSED:
        {
            APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_PRESSED");
            itd_game_scores_write(&scores);
            SCREEN_TRAN(scr_game_in_the_depth_handle, &scr_game_in_the_depth);
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
        }
        break;
        case AC_DISPLAY_BUTTON_UP_PRESSED:
        {
            APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_PRESSED");
            itd_game_scores_write(&scores);
            SCREEN_TRAN(scr_game_ranking_handle, &scr_game_ranking);
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
        }
        break;
        default:
            break;
    }
}
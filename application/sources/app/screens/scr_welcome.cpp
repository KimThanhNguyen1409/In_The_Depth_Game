#include "scr_welcome.h"

static uint8_t w_seagrass_frame = 1;
static int8_t fishes_x[3]       = {-10, -30, -20};
static uint8_t fishes_y[3]      = {25, 35, 42};
static uint8_t fishes_speed[3]  = {2, 1, 3};
static void view_scr_welcome();

view_dynamic_t dyn_view_welcome = {
    {
     .item_type = ITEM_TYPE_DYNAMIC,
     },
    view_scr_welcome
};

view_screen_t scr_welcome = {
    &dyn_view_welcome,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

/*-----------------------------Seagrass animation-----------------------------*/
void itd_game_seagrass_display()
{
    const unsigned char* frames[7] = {
        seagrass_I,
        seagrass_II,
        seagrass_III,
        seagrass_IV,
        seagrass_V,
        seagrass_VI,
        seagrass_VII,
    };
    uint8_t w = SEAGRASS_BITMAP_AXIS_X;
    uint8_t h = SEAGRASS_BITMAP_AXIS_Y;

    view_render.drawBitmap(10, 50, frames[w_seagrass_frame], w, h, WHITE);
    view_render.drawBitmap(54, 46, frames[w_seagrass_frame], w, h, WHITE);
    view_render.drawBitmap(84, 54, frames[w_seagrass_frame], w, h, WHITE);
}

/*-----------------------------Game welcome display-----------------------------*/
void view_scr_welcome()
{
    static uint32_t wel_frame = 0;
    wel_frame++;
    view_render.clear();
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t bub_x = 15 + i * 30;
        uint8_t bub_y = 64 - ((wel_frame * i + 2 * 15) % 64);
        view_render.drawCircle(bub_x, bub_y, 1 + (i % 2), WHITE);
    }
    for (int i = 0; i < 3; i++)
    {
        view_render.drawBitmap(fishes_x[i], fishes_y[i], fish, FISH_BITMAP_AXIS_X, FISH_BITMAP_AXIS_Y, WHITE);
    }
    view_render.drawBitmap(34, 2, title, TITLE_BITMAP_AXIS_X, TITLE_BITMAP_AXIS_Y, WHITE);
    view_render.drawBitmap(0, 58, seabottom, SEABOTTOM_BITMAP_AXIS_X, SEABOTTOM_BITMAP_AXIS_Y, WHITE);
    itd_game_seagrass_display();
    view_render.drawBitmap(30, 48, stone, 20, 20, WHITE);
    view_render.drawBitmap(102, 48, stone, 20, 20, WHITE);
    view_render.setTextColor(WHITE);
    if ((wel_frame / 4) % 2 == 0)
    {
        view_render.setCursor(34, 24);
        view_render.print("PRESS MODE");
    }
}

/*-----------------------------Game welcome handle-----------------------------*/
void scr_welcome_handle(ak_msg_t* msg)
{
    switch (msg->sig)
    {
        case SCREEN_ENTRY:
        {
            APP_DBG_SIG("SCREEN_ENTRY\n");
            itd_game_scores_read(&scores);
            itd_game_setting_read(&settings);
            BUZZER_PlaySound(BUZZER_SOUND_GAME_WELCOME);
            timer_set(AC_TASK_DISPLAY_ID,
                      AC_DISPLAY_WELCOME_TEXT_ANIM_TICK,
                      AC_DISPLAY_WELCOME_TEXT_ANIM_TICK_INTERVAL,
                      TIMER_PERIODIC);
            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_ANIMATION_TICK, 100, TIMER_PERIODIC);
        }
        break;
        case AC_DISPLAY_ANIMATION_TICK:
        {
            APP_DBG_SIG("AC_DISPLAY_ANIAMTION_TICK");
            w_seagrass_frame++;
            if (w_seagrass_frame >= 7)
                w_seagrass_frame = 1;
            for (int i = 0; i < 3; i++)
            {
                fishes_x[i] += fishes_speed[i];
                if (fishes_x[i] > 140)
                {
                    fishes_x[i]     = -10 + (rand() % -30);
                    fishes_y[i]     = 22 + (rand() % 24);
                    fishes_speed[i] = 1 + (rand() % 3);
                }
            }
        }
        break;
        case AC_DISPLAY_WELCOME_TEXT_ANIM_TICK:
        {
            APP_DBG_SIG("AC_DISPLAY_WELCOME_TEXT_ANIM_TICK\n");
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
        }
        break;

        case AC_DISPLAY_BUTTON_MODE_PRESSED:
        {
            APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_PRESSED\n");
            BUZZER_PlaySound(BUZZER_SOUND_WELCOME);
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
            SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
        }
        break;

        case AC_DISPLAY_SHOW_IDLE:
        {
            APP_DBG_SIG("AC_DISPLAY_SHOW_IDLE\n");
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
            SCREEN_TRAN(scr_idle_handle, &scr_idle);
        }
        break;

        case AC_DISPLAY_BUTTON_UP_PRESSED:
        case AC_DISPLAY_BUTTON_DOWN_PRESSED:
        {
            APP_DBG_SIG("AC_DISPLAY_BUTON_%s_PRESSED\n", msg->sig == AC_DISPLAY_BUTTON_UP_PRESSED ? "UP" : "DOWN");
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
            SCREEN_TRAN(scr_qrcode_handle, &scr_qrcode);
        }
        break;

        default:
            break;
    }
}

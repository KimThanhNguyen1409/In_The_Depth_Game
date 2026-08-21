#include "scr_game_setting.h"

static void view_scr_game_setting();
static uint8_t game_setting_index = 0;

view_dynamic_t dyn_view_game_setting = {
    {.item_type = ITEM_TYPE_DYNAMIC},
    view_scr_game_setting,
};

view_screen_t scr_game_setting = {
    &dyn_view_game_setting,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

/*-----------------------------Game setting display-----------------------------*/
void view_scr_game_setting()
{
    static uint32_t bub_colddown = 0;
    bub_colddown++;
    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t bub_x = 5 + i * 30;
        uint8_t bub_y = 64 - ((bub_colddown * i + 2 * 15) % 64);
        view_render.drawCircle(bub_x, bub_y, 1 + (i % 2), WHITE);
    }
    view_render.setTextColor(WHITE);
    view_render.setTextSize(1);
    view_render.setCursor(40, 2);
    view_render.print("SETTING");
    view_render.drawFastHLine(0, 11, 128, WHITE);

    uint8_t y_pos[4] = {15, 29, 43, 55};

    const char* title[4] = {"MODE", "SPEED", "SOUND", "EXIT"};

    for (uint8_t i = 0; i < 4; i++)
    {
        if (game_setting_index == i)
            view_render.drawBitmap(2, y_pos[i] - 2, setting_sub, 16, 12, WHITE);
        if (i < 3)
        {
            view_render.setCursor(22, y_pos[i]);
            view_render.print(title[i]);
        }
        else
        {
            view_render.setCursor(50, y_pos[i]);
            view_render.print(title[i]);
        }
        switch (i)
        {
            case 0:
            {
                for (uint8_t s = 0; s <= settings.mode; s++)
                {
                    view_render.drawBitmap(67 + (s * 14), y_pos[i] - 1, skull, 12, 12, WHITE);
                }
            }
            break;
            case 1:
            {
                view_render.drawRect(67, y_pos[i], 32, 7, WHITE);
                if (settings.speed_mode >= 0)
                    view_render.fillRect(69, y_pos[i] + 2, 8, 3, WHITE);
                if (settings.speed_mode >= 1)
                    view_render.fillRect(79, y_pos[i] + 2, 8, 3, WHITE);
                if (settings.speed_mode >= 2)
                    view_render.fillRect(89, y_pos[i] + 2, 8, 3, WHITE);
            }
            break;
            case 2:
            {
                view_render.drawRoundRect(67, y_pos[i] - 1, 20, 9, 4, WHITE);
                if (settings.sound)
                {
                    view_render.fillCircle(82, y_pos[i] + 3, 3, WHITE);
                }
                else
                {
                    view_render.fillCircle(71, y_pos[i] + 3, 3, WHITE);
                }
                view_render.setCursor(92, y_pos[i]);
                view_render.print(settings.sound ? "ON" : "OFF");
            }
            break;
        }
    }
    view_render.setTextColor(WHITE);
}

/*-----------------------------Game setting handle-----------------------------*/
void scr_game_setting_handle(ak_msg_t* msg)
{
    switch (msg->sig)
    {
        case SCREEN_ENTRY:
        {
            APP_DBG_SIG("SCREEN_ENTRY");
            view_render.clear();
            game_setting_index = SETTING_LOCATION_1;
            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_ANIMATION_TICK, 100, TIMER_PERIODIC);
            itd_game_setting_read(&settings);
        }
        break;
        case AC_DISPLAY_BUTTON_DOWN_PRESSED:
        {
            APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_PRESSED");
            if (game_setting_index < 3)
            {
                game_setting_index++;
            }
            else
            {
                game_setting_index = 3;
            }
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
        }
        break;
        case AC_DISPLAY_BUTTON_UP_PRESSED:
        {
            APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_PRESSED");
            if (game_setting_index > 0)
            {
                game_setting_index--;
            }
            else
            {
                game_setting_index = 0;
            }
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
        }
        break;
        case AC_DISPLAY_BUTTON_MODE_PRESSED:
        {
            APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_PRESSED");
            switch (game_setting_index)
            {
                case SETTING_LOCATION_1:
                {
                    settings.mode++;
                    BUZZER_PlaySound(BUZZER_SOUND_CLICK);
                    if (settings.mode > 2)
                        settings.mode = 0;
                    if (settings.mode == 0)
                    {
                        mainsub.damage_taken = 1;
                    }
                    else if (settings.mode == 1)
                    {
                        mainsub.damage_taken = 2;
                    }
                    else
                    {
                        mainsub.damage_taken = 3;
                    }
                    itd_game_setting_write(&settings);
                }
                break;
                case SETTING_LOCATION_2:
                {
                    settings.speed_mode++;
                    BUZZER_PlaySound(BUZZER_SOUND_CLICK);
                    if (settings.speed_mode > 2)
                        settings.speed_mode = 0;
                    if (settings.speed_mode == 0)
                    {
                        bonus_speed = 0;
                    }
                    else if (settings.speed_mode == 1)
                    {
                        bonus_speed = 4;
                    }
                    else
                    {
                        bonus_speed = 7;
                    }
                    itd_game_setting_write(&settings);
                }
                break;
                case SETTING_LOCATION_3:
                {
                    settings.sound = !settings.sound;
                    BUZZER_PlaySound(BUZZER_SOUND_CLICK);
                    BUZZER_Silent(settings.sound ? BUZZER_SILENT_OFF : BUZZER_SILENT_ON);
                    itd_game_setting_write(&settings);
                }
                break;
                case SETTING_LOCATION_4:
                {
                    itd_game_setting_write(&settings);
                    BUZZER_PlaySound(BUZZER_SOUND_CLICK);
                    SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
                    game_setting_index = 0;
                }
                break;
                default:
                    break;
            }
        }
        break;
        default:
            break;
    }
}

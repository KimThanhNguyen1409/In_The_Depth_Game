#include "scr_game_setting.h"

static void view_scr_game_setting();
static uint8_t game_setting_index = 0;
static const char *mode[] = {
    "NOR",
    "HARD",
    "EXPERT"
};
static const char *speed[] = {
    "NOR",
    "FAST",
    "FLASH"
};
static const char *sound[] = {
    "ON",
    "OFF"
};
static uint8_t mode_index = 0;
static uint8_t speed_index = 0;
static uint8_t sound_index = 0;
view_dynamic_t dyn_view_game_setting = {
    {
    .item_type = ITEM_TYPE_DYNAMIC
    },
    view_scr_game_setting
};

view_screen_t scr_game_setting = {
    &dyn_view_game_setting,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

void view_scr_game_setting(){
    view_render.setTextSize(1);
    if(game_setting_index == 0)
    {
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X, 
                                    0, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_W, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_H, 
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X + 1, 
                                    1, 
                                    AC_DISPLAY_ROUNDRECT_CHOOSE_W, 
                                    AC_DISPLAY_ROUNDRECT_CHOOSE_H, 
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.setCursor(AC_DISPLAY_TEXT_AXIS_X, 4);
        view_render.print("MODE    [       ]");
        view_render.setCursor(71, 4);
        view_render.print(mode[mode_index]);
    }
    else
    {
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X, 
                                    0, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_W, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_H, 
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.setCursor(AC_DISPLAY_TEXT_AXIS_X, 4);
        view_render.print("MODE    [       ]");
        view_render.setCursor(71, 4);
        view_render.print(mode[mode_index]);
    }
    if(game_setting_index == 1)
    {
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X, 
                                    16, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_W, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_H, 
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X + 1, 
                                    17, 
                                    AC_DISPLAY_ROUNDRECT_CHOOSE_W, 
                                    AC_DISPLAY_ROUNDRECT_CHOOSE_H, 
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.setCursor(AC_DISPLAY_TEXT_AXIS_X, 20);
        view_render.print("SPEED   [       ]");
        view_render.setCursor(71, 20);
        view_render.print(speed[speed_index]);
    }
    else
    {
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X, 
                                    16, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_W, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_H, 
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.setCursor(AC_DISPLAY_TEXT_AXIS_X, 20);
        view_render.print("SPEED   [       ]");
        view_render.setCursor(71, 20);
        view_render.print(speed[speed_index]);
    }
    if(game_setting_index == 2)
    {
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X, 
                                    32, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_W, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_H, 
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X + 1, 
                                    33, 
                                    AC_DISPLAY_ROUNDRECT_CHOOSE_W, 
                                    AC_DISPLAY_ROUNDRECT_CHOOSE_H, 
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.setCursor(AC_DISPLAY_TEXT_AXIS_X, 36);
        view_render.print("SOUND      [    ]");
        view_render.setCursor(88 ,36);
        view_render.print(sound[sound_index]);
    }
    else
    {
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X, 
                                    32, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_W, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_H,  
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.setCursor(AC_DISPLAY_TEXT_AXIS_X, 36);
        view_render.print("SOUND      [    ]");
        view_render.setCursor(88 ,36);
        view_render.print(sound[sound_index]);
    }
    if(game_setting_index == 3)
    {
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X, 
                                    48, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_W, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_H, 
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X + 1, 
                                    49, 
                                    AC_DISPLAY_ROUNDRECT_CHOOSE_W, 
                                    AC_DISPLAY_ROUNDRECT_CHOOSE_H, 
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.setCursor(AC_DISPLAY_TEXT_AXIS_X, 52);
        view_render.print("EXIT AND SAVE");
    }
    else
    {
        view_render.drawRoundRect(  AC_DISPLAY_ROUNDRECT_AXIS_X, 
                                    48, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_W, 
                                    AC_DISPLAY_ROUNDRECT_FRAME_H, 
                                    AC_DISPLAY_ROUNDRECT_RADIUS, 
                                    WHITE);
        view_render.setCursor(AC_DISPLAY_TEXT_AXIS_X, 52);
        view_render.print("EXIT AND SAVE");
    }
}
void scr_game_setting_handle(ak_msg_t *msg){
    switch (msg->sig)
    {
    case AC_DISPLAY_BUTTON_DOWN_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTTON_DOWN_PRESSED");
        if(game_setting_index < 3){
            game_setting_index++; 
        }else{
            game_setting_index = 3;
        }
    }
    break;
    case AC_DISPLAY_BUTTON_UP_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTTON_UP_PRESSED");
        if(game_setting_index > 0){
            game_setting_index--; 
        }else{
            game_setting_index = 0;
        }
    }
    break;
    case AC_DISPLAY_BUTTON_MODE_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTTON_MODE_PRESSED");
        switch (game_setting_index)
        {
        case SETTING_LOCATION_1:
        {
           mode_index++;
            if(mode_index > 2)
                mode_index = 0;
            if(mode_index == 0){
                mainsub.damage_taken = 1;
            }else if(mode_index == 1){
                mainsub.damage_taken = 2;
            }else{
                mainsub.damage_taken = 3;
            }
        }
        break;
        case SETTING_LOCATION_2:
        {
            speed_index++;
            if(speed_index > 2)
                speed_index = 0;
            if(speed_index == 0){
                bonus_speed = 0;
            }else if(speed_index == 1){
                bonus_speed = 4;
            }else{
                bonus_speed = 7;
            }
        }
        break;
        case SETTING_LOCATION_3:
        {
            sound_index++;
            if(sound_index  > 1)
                sound_index = 0;
        }
        break;
        case SETTING_LOCATION_4:
        {
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




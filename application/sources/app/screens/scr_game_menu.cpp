#include "scr_game_menu.h"

static void view_scr_game_menu();
static uint8_t game_menu_index = 0;
view_dynamic_t dyn_view_game_menu = {
    {
    .item_type = ITEM_TYPE_DYNAMIC
    },
    view_scr_game_menu
};
view_screen_t scr_game_menu = {
    &dyn_view_game_menu,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

void view_scr_game_menu(){
    view_render.setTextSize(1);
    view_render.setCursor(26, 4);
    view_render.print("IN THE DEPTH");
    view_render.drawFastHLine(0, 14, 128, WHITE);
    if(game_menu_index == 0){
        view_render.drawRoundRect(4, 17, 30, 30, 4, WHITE);
        view_render.drawRoundRect(6, 19, 26, 26, 4, WHITE);
        view_render.drawBitmap(9, 22, main_sub, MAINSUB_SIZE_BITMAP_X, MAINSUB_SIZE_BITMAP_Y, WHITE);
        view_render.setCursor(54, 56);
        view_render.print("DIVE");
    }else{
        view_render.drawRoundRect(4, 17, 30, 30, 4, WHITE);
        view_render.drawBitmap(9, 22, main_sub, MAINSUB_SIZE_BITMAP_X, MAINSUB_SIZE_BITMAP_Y, WHITE);
    }
    if(game_menu_index == 1){
        view_render.drawRoundRect(34, 17, 30, 30, 4, WHITE);
        view_render.drawRoundRect(36, 19, 26, 26, 4, WHITE);
        view_render.drawBitmap(38, 17, setting, 24, 24, WHITE);
        view_render.setCursor(48, 56);
        view_render.print("SETTING");
    }else{
        view_render.drawRoundRect(34, 17, 30, 30, 4, WHITE);
        view_render.drawBitmap(38, 17, setting, 24, 24, WHITE);
    }
    if(game_menu_index == 2){
        view_render.drawRoundRect(64, 17, 30, 30, 4, WHITE);
        view_render.drawRoundRect(66, 19, 26, 26, 4, WHITE);
        view_render.drawBitmap(64, 24, rank, 30, 16, WHITE);
        view_render.setCursor(54, 56);
        view_render.print("RANK");
    }else{
        view_render.drawRoundRect(64, 17, 30, 30, 4, WHITE);
        view_render.drawBitmap(64, 24, rank, 30, 16, WHITE);
    }
    if(game_menu_index == 3){
        view_render.drawRoundRect(94, 17, 30, 30, 4, WHITE);
        view_render.drawRoundRect(96, 19, 26, 26, 4, WHITE);
        view_render.drawBitmap(98, 19, exit_icon, 24, 24, WHITE);
        view_render.setCursor(54, 56);
        view_render.print("EXIT");
    }else{
        view_render.drawRoundRect(94, 17, 30, 30, 4, WHITE);
        view_render.drawBitmap(98, 19, exit_icon, 24, 24, WHITE);
    }
    view_render.drawFastHLine(0, 52, 128, WHITE);
    view_render.drawBitmap(8, 48, grass, 16, 16, WHITE);
    view_render.drawBitmap(102, 48, grass, 16, 16, WHITE);
}
void scr_game_menu_handle(ak_msg_t *msg){
    switch (msg->sig)
    {
    case AC_DISPLAY_BUTTON_UP_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTTON_UP_PRESSED");
        if(game_menu_index < 3){
            game_menu_index++; 
        }else{
            game_menu_index = 3;
        }
    }
    break;
    case AC_DISPLAY_BUTTON_DOWN_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTTON_DOWN_PRESSED");
        if(game_menu_index > 0){
            game_menu_index--;
        }else{
            game_menu_index = 0;
        }
    }
    break;
    case AC_DISPLAY_BUTTON_MODE_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTTON_MODE_PRESSED");
        switch (game_menu_index)
        {
        case MENU_LOCATION_1:
        {
            SCREEN_TRAN(scr_game_in_the_depth_handle, &scr_game_in_the_depth);
        }
        break;
        case MENU_LOCATION_2:
        {
            SCREEN_TRAN(scr_game_setting_handle, &scr_game_setting);
        }
        break;
        case MENU_LOCATION_3:
        break;
        case MENU_LOCATION_4:
        {
            SCREEN_TRAN(scr_welcome_handle, &scr_welcome);
            game_menu_index = 0;
        }
        break;
        default:
        break;
        }
        break;
    }
        default:
            break;
    }
}



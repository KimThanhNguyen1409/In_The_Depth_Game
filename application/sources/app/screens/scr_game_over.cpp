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

void view_scr_game_over(){
    view_render.drawRect(8, 4, 104, 40, WHITE);
    view_render.drawRect(8, 44, 34, 20, WHITE);
    view_render.drawBitmap(9, 45, retry, 30, 16, WHITE);
    view_render.drawRect(43, 44, 34, 20, WHITE);
    view_render.drawBitmap(44, 45, rank, 30, 16, WHITE);
    view_render.drawRect(78, 44, 34, 20, WHITE);
    view_render.drawBitmap(79, 45, home, 30, 16, WHITE);
    view_render.setCursor(16, 8);
    view_render.print("SCORE:");
    view_render.setCursor(50, 8);
    view_render.print(itd_game_score);
    view_render.setCursor(16, 16);
    view_render.print("TIME:");
    view_render.setCursor(50, 16);
    view_render.print(itd_game_time);
    for(uint8_t i = 1; i < 4; i++){
        view_render.drawBitmap(16 * i + 8, 20, fish, 32, 32, WHITE);
    }
}
void scr_game_over_handle(ak_msg_t *msg){
    switch (msg->sig)
    {
    case SCREEN_ENTRY:
    {
        APP_DBG_SIG("SCREEN_ENTRY\n");
        view_render.initialize();
        view_render_display_on();    
    }
    break;
    case AC_DISPLAY_BUTTON_MODE_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_PRESSED");
        SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
    }
    break;
    case AC_DISPLAY_BUTTON_DOWN_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_PRESSED");
        SCREEN_TRAN(scr_game_in_the_depth_handle, &scr_game_in_the_depth);
    }
    break;
    case AC_DISPLAY_BUTTON_UP_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_PRESSED");
        SCREEN_TRAN(scr_game_in_the_depth_handle, &scr_game_in_the_depth);
    }
    break;
    default:
        break;
    }
}
#include "scr_game_ranking.h"

#define RANKSUB_BITMAP_SIZE_X   (48) 
#define RANKSUB_BITMAP_SIZE_Y   (32)

static void view_scr_game_ranking();
static uint8_t grid_offset_x = 0;
static uint8_t c1_y = 32; static int8_t c1_dir = 1;                                                                                    
static uint8_t c2_y = 24; static int8_t c2_dir = -1;                                                
static uint8_t c3_y = 32; static int8_t c3_dir = 1;  
view_dynamic_t dyn_view_item_game_ranking = {
    {
    .item_type = ITEM_TYPE_DYNAMIC
    },
    view_scr_game_ranking
};

view_screen_t scr_game_ranking = {
    &dyn_view_item_game_ranking,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

void view_scr_game_ranking(){
    view_render.setTextSize(1);
    view_render.setTextColor(BLACK);
    const uint8_t c_x[3] = {20, 64, 108};
    const uint8_t c_r[3] = {16, 22, 16};
    const uint8_t c_y[3] = {c1_y , c2_y, c3_y}; 
    const uint8_t y_off[3] = {8, 10, 8};
    const uint8_t chain_x[3] = {14, 58, 102};
    const uint8_t text_x[3] = {10, 58, 98};
    const uint16_t text_score[3] = {scores.score_2st, scores.score_1st, scores.score_3rd};
    const char *text[] = {"2nd", "1st", "3nd"};
    for(uint8_t j = 0; j < 3; j ++){
        view_render.fillCircle(c_x[j], c_y[j], c_r[j], WHITE);
        view_render.drawBitmap(chain_x[j], c_y[j] + y_off[j], chain, 10, 40, WHITE);
        view_render.setCursor(text_x[j], c_y[j] - y_off[j]);
        view_render.print(text[j]);
        view_render.setCursor(text_x[j], c_y[j]);
        view_render.print(text_score[j]);
    }
   
    for(uint8_t i = 0; i <= 128; i+=16){
        for(uint8_t j = 0; j <= 64; j +=16){
            view_render.drawPixel(i - grid_offset_x, j, WHITE);
        }
    }
}
void scr_game_ranking_handle(ak_msg_t *msg){
    switch (msg->sig)
    {
    case SCREEN_ENTRY:
    {
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_ANIMATION_TICK, 100, TIMER_PERIODIC);
    }
    break;
    case AC_DISPLAY_ANIMATION_TICK:
    {
        grid_offset_x++;
        if(grid_offset_x >= 16)
            grid_offset_x = 0;
        c1_y += c1_dir;
        if (c1_y >= 36 || c1_y <= 28) c1_dir = -c1_dir; 
        c2_y += c2_dir;
        if (c2_y >= 28 || c2_y <= 20) c2_dir = -c2_dir;             
        c3_y += c3_dir;
        if (c3_y >= 36 || c3_y <= 28) c3_dir = -c3_dir;

    }
    break;
    case AC_DISPLAY_BUTTON_MODE_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTTON_MODE_PRESSED");
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_ANIMATION_TICK);
        SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
    }
    break;
    case AC_DISPLAY_BUTTON_UP_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTTON_UP_PRESSED");
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_ANIMATION_TICK);
        SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
    }
    break;
    case AC_DISPLAY_BUTTON_DOWN_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTTON_DOWN_PRESSED");
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_ANIMATION_TICK);
        SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
    }
    break;
    default:
        break;
    }
}
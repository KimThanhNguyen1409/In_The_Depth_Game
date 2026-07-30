#include "scr_game_menu.h"


static int8_t fishes_x[3] = {-10, -30, -20};                                                                                        
static uint8_t fishes_y[3] = {45, 50, 52};                                                                                           
static uint8_t fishes_speed[3] = {2, 1, 3};                                                                                          
static uint8_t m_seagrass_frame = 1;  
uint8_t bub_x = rand() % 128;
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
void menu_seagrass_display(){                                                                                          
    const unsigned char *frame = seagrass_I;         
    uint8_t w = SEAGRASS_BITMAP_AXIS_X;
    uint8_t h = SEAGRASS_BITMAP_AXIS_Y;                                                                                  
    if (m_seagrass_frame == 2) frame = seagrass_II;                                                                                    
    else if (m_seagrass_frame == 3) frame = seagrass_III;                                                                              
    else if (m_seagrass_frame == 4) frame = seagrass_IV;                                                                               
    else if (m_seagrass_frame == 5) frame = seagrass_V;                                                                                
    else if (m_seagrass_frame == 6) frame = seagrass_VI;                                                                               
    else if (m_seagrass_frame == 7) frame = seagrass_VII;                                                                              
                                                                                                                                                                     
    view_render.drawBitmap(54, 46, frame, w, h, WHITE);                           
} 
void view_scr_game_menu(){
    static uint32_t bub_colddown = 0;
    bub_colddown++;
	for(uint8_t i = 0; i < 5; i++){
		uint8_t bub_x = 5 + i * 30;
		uint8_t bub_y = 64 - ((bub_colddown * i + 2 * 15) % 64);
		view_render.drawCircle(bub_x, bub_y, 1 + (i % 2), WHITE);
    }
    view_render.drawBitmap(0, 58, seabottom, SEABOTTOM_BITMAP_AXIS_X, SEABOTTOM_BITMAP_AXIS_Y, WHITE);
    const uint8_t *title[4] = {dive_letter, setting_letter, rank_letter, exit_letter};                                                                        
    const uint8_t text_w[4] = {32, 58, 32, 32};    
    const uint8_t text_h = 7;                                                                                                                                                                                         
    const uint8_t c_x[4] = {15, 46, 78, 110};                                                                                          
    const uint8_t c_y[4] = {34, 24, 30, 24};                                                                                           
    const uint8_t chain_x[4] = {10, 40, 72, 106};                                                                                      
    const uint8_t chain_y[4] = {42, 32, 40, 32};                                                                                                                                                                                                                              
    const uint8_t *icons[4] = {main_sub, setting, rank, exit_icon};                                                                    
    const uint8_t icons_w[4] = {MAINSUB_SIZE_BITMAP_X, SETTING_BITMAP_AXIS_X, RANK_BITMAP_AXIS_X, EXIT_BITMAP_AXIS_X};                 
    const uint8_t icons_h[4] = {MAINSUB_SIZE_BITMAP_Y, SETTING_BITMAP_AXIS_Y, RANK_BITMAP_AXIS_Y, EXIT_BITMAP_AXIS_Y};                                                                                                                                                            
    for(uint8_t i = 0; i < 4; i++){                                                                                                    
        view_render.drawBitmap(chain_x[i], chain_y[i], chain, 10, 40, WHITE);     
        int8_t off_x[4] = {0, 0, 0, 2};
        int8_t off_y[4] = {-1, -2, 0, -1};                                                                                                                                                                                                
        uint8_t imgx = c_x[i] - icons_w[i] / 2 + off_x[i];                                                                                        
        uint8_t imgy = c_y[i] - icons_h[i] / 2 + off_y[i];                                                                                                                                                                                                                               
        if (game_menu_index == i) {                                                                                                                                                                    
            view_render.fillCircle(c_x[i], c_y[i], 15, WHITE);                                                                         
            view_render.drawBitmap(imgx, imgy, icons[i], icons_w[i], icons_h[i], BLACK);            
            uint8_t text_x = (128 - text_w[i]) / 2; 
            view_render.drawBitmap(text_x, 1, title[i], text_w[i], text_h, WHITE);                                                                                                                                                      
        } else {                                                                                                                                                                  
            view_render.fillCircle(c_x[i], c_y[i], 14, BLACK);                                                                         
            view_render.drawCircle(c_x[i], c_y[i], 15, WHITE);
            view_render.drawBitmap(imgx, imgy, icons[i], icons_w[i], icons_h[i], WHITE);
        }
    }
    for(uint8_t i = 0; i < 3; i++) {                                                                                                       
        view_render.drawBitmap(fishes_x[i], fishes_y[i], fish, FISH_BITMAP_AXIS_X, FISH_BITMAP_AXIS_Y, WHITE);                     
    }       
    menu_seagrass_display();
}
void scr_game_menu_handle(ak_msg_t *msg){
    switch (msg->sig)
    {   
    case SCREEN_ENTRY:
    {
        APP_DBG_SIG("SCREEN_ENTRY");
        itd_game_setting_read(&settings);
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_ANIMATION_TICK, 100, TIMER_PERIODIC);
    }   
    break;
    case AC_DISPLAY_ANIMATION_TICK:
    {
        m_seagrass_frame++;                                                                                                            
        if(m_seagrass_frame >= 7) m_seagrass_frame = 1;                                                                                                                                                                                                                   
		for(int i = 0; i < 3; i++) {                                                                                                   
            fishes_x[i] += fishes_speed[i];                                                                                                                                              
            if (fishes_x[i] > 140) {                                                                                                   
                fishes_x[i] = -10 + (rand() % -30);                                                                                                              
                fishes_y[i] = 22 + (rand() % 24);                                                                                      
                fishes_speed[i] = 1 + (rand() % 3);                                                                  
            }                                                                                                                          
        }  
    }
    break;
    case AC_DISPLAY_BUTTON_UP_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTTON_UP_PRESSED");
        if(game_menu_index < 3){
            game_menu_index++; 
        }else{
            game_menu_index = 3;
        }
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
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
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
    }
    break;
    case AC_DISPLAY_BUTTON_MODE_PRESSED:
    {
        APP_DBG_SIG("AC_DISPLAY_BTTON_MODE_PRESSED");
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_ANIMATION_TICK);
        switch (game_menu_index)
        {
        case MENU_LOCATION_1:
        {
            BUZZER_PlaySound(BUZZER_SOUND_LETS_GO);
            SCREEN_TRAN(scr_game_in_the_depth_handle, &scr_game_in_the_depth);
        }
        break;
        case MENU_LOCATION_2:
        {   
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            SCREEN_TRAN(scr_game_setting_handle, &scr_game_setting);
        }
        break;
        case MENU_LOCATION_3:
        {
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            SCREEN_TRAN(scr_game_ranking_handle, &scr_game_ranking);
        }
        break;
        case MENU_LOCATION_4:
        {
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
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



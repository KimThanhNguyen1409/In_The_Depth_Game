#include "scr_welcome.h"

#define SEAGRASS_BITMAP_AXIS_X (20)
#define SEAGRASS_BITMAP_AXIS_Y (40)
#define SEAGRASS_NUMBER (7)

typedef struct{
	uint8_t x;
	uint8_t y;
	uint8_t action_image;
} itd_game_seagrass_t;

static itd_game_seagrass_t seagrasses[SEAGRASS_NUMBER];

static void view_scr_welcome();

view_dynamic_t dyn_view_welcome = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_welcome};

view_screen_t scr_welcome = {
	&dyn_view_welcome,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};
void itd_game_seagrass_display(){
	uint8_t i = 0;
	for (i = 0; i < SEAGRASS_NUMBER; i++)
    {

        const unsigned char *frame = seagrass_I;
        uint8_t w = SEAGRASS_BITMAP_AXIS_X;
        uint8_t h = SEAGRASS_BITMAP_AXIS_Y;
        if (seagrasses[i].action_image == 2)
        {
            frame = seagrass_II;
        }
        else if (seagrasses[i].action_image == 3)
        {
            frame = seagrass_III;
        }
        else if (seagrasses[i].action_image == 4)
        {
            frame = seagrass_IV;
        }
        else if (seagrasses[i].action_image == 5)
        {
            frame = seagrass_V;
        }
        else if (seagrasses[i].action_image == 6)
        {
            frame = seagrass_VI;
        }
		else if (seagrasses[i].action_image == 7)
		{
			frame = seagrass_VII;
			i = 0;
		}
        view_render.drawBitmap(50, 50, frame, w, h, WHITE);
		seagrasses->action_image++;
		
    }
}
void view_scr_welcome()
{
	view_render.clear();
	view_render.drawFastHLine(0, 40, 128, WHITE);
	view_render.drawBitmap(10, 45, stone, 20, 20, WHITE);
	view_render.drawBitmap(100, 40, stone, 20, 20, WHITE);
	view_render.drawBitmap(50, 38, crab, 32 ,32, WHITE);
	view_render.setTextSize(1);
    view_render.setCursor(24, 4);
    view_render.print("IN THE DEPTH");
	view_render.setCursor(24, 8);
	
}

void scr_welcome_handle(ak_msg_t *msg)
{
	switch (msg->sig)
	{
	case SCREEN_ENTRY:
	{
		APP_DBG_SIG("SCREEN_ENTRY\n");
		// BUZZER_PlaySound(BUZZER_SOUND_WELCOME);
		timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK_INTERVAL, TIMER_PERIODIC);
		timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);
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
		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_WELCOME_TEXT_ANIM_TICK);
		// SCREEN_TRAN(scr_game_in_the_depth_handle, &scr_game_in_the_depth);
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

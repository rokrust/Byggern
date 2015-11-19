#ifndef _INTERFACE_
#define _INTERFACE_

#include "../Drivers/MultifunctionCard/joystick/joy.h"
#include "../Drivers/MultifunctionCard/oled/oled.h"


typedef struct Menu Menu;
struct Menu{
	uint8_t num_submenus;
	Menu **submenus;
	Menu *prev;
	char *title;
	void (*action)();
	
};

Menu* new_Menu(char* title, void(*action)(void), uint8_t num_submenus);
void interface_draw_selector(int row);
void interface_print(Menu* menu);
void interface_select();
void interface_init();
Menu* interface_get_current_menu();

#endif
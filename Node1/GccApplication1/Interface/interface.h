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
void draw_selector(int select_pos);
void interface_print(Menu* menu);
void interface_select(Joystick joy, int *select_pos, Menu *(*current_menu));
Menu* interface_init();

#endif
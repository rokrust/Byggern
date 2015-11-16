#include "interface.h"
#include "../Game/highscore.h"
#include "../Fonts/font.h"

#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

Menu* interface_init(){
	Menu *mainMenu = new_Menu("Main menu", NULL, 2);
	Menu *menu1 = new_Menu("Screen options", NULL, 1);
	Menu *menu2 = new_Menu("High scores", NULL, 1);
	Menu *menu11 = new_Menu("Party mode", oled_initiate_party_mode, 0);
	mainMenu->submenus[0] = menu1, mainMenu->submenus[1] = menu2;
	menu1->submenus[0] = menu11, menu1->prev = mainMenu;
	menu2->prev = mainMenu;
	menu11->submenus = NULL;
	
	
	interface_print(mainMenu);
	return mainMenu;
}
Menu* new_Menu(char* title, void(*action)(void), uint8_t num_submenus){
	Menu* m = malloc(sizeof(Menu));
	
	m->title = title;
	m->num_submenus = num_submenus;
	m->action = action;
	m->submenus = malloc(sizeof(Menu)*num_submenus);
	return m;
}
void interface_print(Menu *menu){
	oled_clear_screen();
	oled_set_start_col(0);
	oled_write_string(menu->title, 0);
	
	for(int i = 0; i < menu->num_submenus; i++){
		oled_set_start_col(10);
		oled_write_string(menu->submenus[i]->title, i + 1);
	}
	
	interface_draw_selector(1);
	
}
void interface_draw_selector(int select_pos){
	oled_set_start_col(0);
	oled_write(' ');
	
	oled_set_start_col(0);
	oled_control_assign(0xb0 + select_pos);
	oled_write_selector();
	
	oled_set_start_col(8);
}
void interface_select(Joystick joy, int *select_pos, Menu *(*current_menu)){
	direction previous_direction = NEUTRAL;

	while(JOY_get_direction(joy) != NEUTRAL){
		previous_direction = JOY_get_direction(joy);
		JOY_read_joystick(&joy);
	} //Wait for joy to reach neutral position.
	
	switch(previous_direction){
		case UP:
			if(*select_pos != 1){
				(*select_pos)--;
				interface_draw_selector(*select_pos);
			}
			break;
			
		case DOWN:
			if((*current_menu)->num_submenus != *select_pos){
				(*select_pos)++;
				interface_draw_selector(*select_pos);
			}
			break;
			
		case RIGHT:
			if((*current_menu)->submenus[*select_pos-1]->num_submenus){
				(*current_menu) = (*current_menu)->submenus[*select_pos-1];
				interface_print((*current_menu));
				*select_pos = 1;
				
			}
			else{
				(*current_menu)->submenus[*select_pos-1]->action();
			}
			
			break;
			
		case LEFT:
			if((*current_menu)->prev){
				*select_pos = 1;
				(*current_menu) = (*current_menu)->prev;
				interface_print((*current_menu));
			}
			break;
		case NEUTRAL:
			break;
		
	}
}
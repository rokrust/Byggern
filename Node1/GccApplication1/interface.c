#include "interface.h"
#include "font.h"
#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

Menu* interface_init(){
	Menu *mainMenu = new_Menu("Main menu", NULL, 2);
	Menu *menu1 = new_Menu("Screen options", NULL, 1);
	Menu *menu2 = new_Menu("Other options", NULL, 1);
	Menu *menu11 = new_Menu("Party mode", oled_initiate_party_mode, 0);
	mainMenu->submenus[0] = menu1, mainMenu->submenus[1] = menu2;
	menu1->submenus[0] = menu11, menu1->prev = mainMenu;
	menu2->prev = mainMenu;
	menu11->submenus = NULL;
	
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
	printf("Title: %s\n", menu->title);
	oled_write_string(menu->title, 0);
	
	for(int i = 0; i < menu->num_submenus; i++){
		oled_set_start_col(10);
		oled_write_string(menu->submenus[i]->title, i + 1);
	}
	
	draw_selector(1);
	
}
void draw_selector(int select_pos){
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
		//printf("Direction: %d\n", JOY_get_direction(joy));
	} //Wait for joy to reach neutral position.
	switch(previous_direction){
		case UP:
			if(*select_pos != 1){
				printf("case UP\n");
				(*select_pos)--;
				draw_selector(*select_pos);
			}
			break;
			
		case DOWN:
			printf("In down.\n");
			printf("Select pos: %d\n", *select_pos);
			if((*current_menu)->num_submenus != *select_pos){
				(*select_pos)++;
				draw_selector(*select_pos);
			}
			printf("Out of down\n");
			break;
			
		case RIGHT:
			printf("case RIGHT\n");
			if((*current_menu)->submenus[*select_pos-1]->num_submenus){
				printf("Submenu found!\n");
				(*current_menu) = (*current_menu)->submenus[*select_pos-1];
				interface_print((*current_menu));
				*select_pos = 1;
				
			}
			else{
				printf("ACTION!");
				(*current_menu)->submenus[*select_pos-1]->action();
			}
			
			break;
			
		case LEFT:
			if((*current_menu)->prev){
				printf("case LEFT\n");
				*select_pos = 1;
				(*current_menu) = (*current_menu)->prev;
				interface_print((*current_menu));
			}
			break;
		case NEUTRAL:
			break;
		
	}
}
#include "interface.h"
#include "../Game/highscore.h"
#include "../Game/game.h"
#include "../Fonts/font.h"
#include "../Drivers/Communication/can/can.h"


#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>


int select_pos;
Menu* current_menu;

void interface_init(){
	Menu *mainMenu = new_Menu("Main menu", NULL, 4);
	
	Menu *menu0 = new_Menu("Play game!", game_main, 0);
	
	Menu *menu1 = new_Menu("High scores!", highscore_print, 0);
	
	Menu *menu2 = new_Menu("Screen options", NULL, 1);
	Menu *menu21 = new_Menu("Brightness", oled_set_brightness, 0);
	
	Menu *menu3 = new_Menu("Game options", NULL, 2);
	Menu *menu31 = new_Menu("Set difficulty", NULL, 3);
	Menu *menu32 = new_Menu("Reset Hi-scores", highscore_resetAll, 0);
	Menu *menu311 = new_Menu("Medium", game_medium_difficulty, 0);
	Menu *menu312 = new_Menu("Hard", game_hard_difficulty, 0);
	Menu *menu313 = new_Menu("Impossible", game_impossible_difficulty, 0);
	
	mainMenu->submenus[0] = menu0, mainMenu->submenus[1] = menu1;
	mainMenu->submenus[2] = menu2, mainMenu->submenus[3] = menu3;
	mainMenu->prev = NULL;
	
	menu0->submenus = NULL, menu0->prev = NULL;
	
	menu2->submenus[0] = menu21;
	menu2->prev = mainMenu;
	menu21->submenus = NULL;
	
	//Set difficulty
	menu3->prev = mainMenu;
	menu3->submenus[0] = menu31, menu3->submenus[1] = menu32;
	menu31->submenus[0] = menu311, menu31->submenus[1] = menu312, menu31->submenus[2] = menu313;
	menu31->prev = mainMenu;
	menu32->submenus=NULL;
	menu311->submenus = NULL, menu312->submenus = NULL, menu313->submenus = NULL;
	
	select_pos = 1;
	current_menu = mainMenu;
	
	interface_print(mainMenu);
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
void interface_draw_selector(int pos){
	oled_set_start_col(0);
	oled_write(' ');
	
	oled_set_write_position(pos, 0);
	oled_write_selector();
	
	oled_set_start_col(8);
}
void interface_select(){
	direction previous_direction = NEUTRAL;
		
	while(joy_get_direction() != NEUTRAL){
		previous_direction = joy_get_direction();
	} //Wait for joy to reach neutral position.
	
	switch(previous_direction){
		case UP:
			//Move selector up if it's not on line 0.
			if(select_pos != 1){
				select_pos--;
				interface_draw_selector(select_pos);
			}
			break;
			
		case DOWN:
			//Move selector down if it's not on line 7.
			if(current_menu->num_submenus != select_pos){
				(select_pos)++;
				interface_draw_selector(select_pos);
			}
			break;
			
		case RIGHT:
			//Check for Menu pointer and go to nex Menu.
			if(current_menu->submenus[select_pos-1]->num_submenus){
				current_menu = current_menu->submenus[select_pos-1];
				interface_print(current_menu);
				select_pos = 1;
				
			}
			//Found function pointer.
			else{
				current_menu->submenus[select_pos-1]->action();
			}
			
			break;
			
		case LEFT:
			//Go to previous menu.
			if(current_menu->prev){
				select_pos = 1;
				current_menu = current_menu->prev;
				interface_print(current_menu);
			}
			break;
		case NEUTRAL:
			break;
		
	}
}
Menu* interface_get_current_menu(){
	return current_menu;
}
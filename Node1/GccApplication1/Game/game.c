#include "game.h"
#include "highscore.h"
#include "../Drivers/Sensors/ir.h"
#include "../Drivers/timer/timer.h"
#include "../Interface/interface.h"
#include "../Drivers/Communication/can/can.h"
#include "../Drivers/Communication/can/mcp/mcp.h"
#include "../Drivers/Communication/can/mcp/mcp_defines.h"

#include <util/delay.h>

uint8_t game_score;
uint8_t game_score_counter;
uint8_t counting_speed;

void game_init(uint8_t speed){
	counting_speed = speed;
}
void game_start(void){
	game_lost = 0;
	timer_enable();
	game_reset_score();
	oled_clear_screen();
}

void game_track_score(){
	game_score_counter++;
	
	//Using 70 Hz clock
	if(game_score_counter > counting_speed){
		game_score++;
		game_score_counter = 0;
		oled_set_write_position(4, 16);
		oled_printf("Score: %d", game_score);
	}
}

void game_end(void){
	timer_disable();
	oled_clear_screen();
	uint8_t rank = highscore_update(game_score);
	if(rank){
		game_postGameMessage(rank);
		_delay_ms(5000);
	
	}
	else{
		oled_set_write_position(2, 8);	
		printf("You so bad.");
		_delay_ms(5000);
	}
	
	highscore_print();
}

void game_reset_score(void){
	game_score = 0;
	game_score_counter = 0;
}
void game_introMessage(void){
	oled_clear_screen();
	oled_set_write_position(0, 0);
	oled_printf("Welcome to this");
	oled_set_write_position(1, 0);
	oled_printf("interactive ball");
	oled_set_write_position(2, 0);
	oled_printf("bump experience.");
	oled_set_write_position(4, 0);
	oled_printf("Put your balls");
	oled_set_write_position(5, 0);
	oled_printf("on the solenoid");
	oled_set_write_position(6, 0);
	oled_printf("to begin..");
	oled_set_write_position(7, 0);
	oled_printf("  ..good luck");
}

void game_postGameMessage(int rank){
	oled_set_write_position(1,8);
	oled_printf("NEW HIGHSCORE!");
	oled_set_write_position(2,0);
	oled_printf("Finished as");
	oled_set_write_position(3,0);
	oled_printf("number %d", rank);
}


void game_main(void){
	game_introMessage();
	while(!joy_read_right_button()){
		_delay_us(10);
	}
	game_start();
	while(!game_lost){
		_delay_us(10);
	}
	oled_printf("Game lost!");
	game_end();
}

void game_hard_difficulty(void){
	game_init(35);
	can_send_pid_message(7, 1, 1, 1, 0, 0);
}

void game_medium_difficulty(void){
	game_init(70);
	can_send_pid_message(1, 1, 2, 1, 0, 0);
}

void game_impossible_difficulty(void){
	game_init(10);
	can_send_pid_message(1, 2, 1, 2, 1, 1);
	
}



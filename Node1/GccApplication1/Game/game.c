#include "game.h"
#include "highscore.h"
#include "../Drivers/Sensors/ir.h"
#include "../Interface/interface.h"
#include "../Drivers/timer/timer.h"

uint8_t game_score;
uint8_t game_score_counter;

void game_start(void){
	timer_enable();
	game_reset_score();
	oled_clear_screen();
}

void game_track_score(){
	game_score_counter++;
	
	//Using 70 Hz clock
	if(game_score_counter > 7){
		game_score++;
		game_score_counter = 0;
		oled_set_start_col(16);
		oled_printf("%d", game_score);
	}
}

void game_end(void){
	timer_disable();
	oled_clear_screen();
	highscore_update(game_score);
}

void game_reset_score(void){
	game_score = 0;
	game_score_counter = 0;
}

void game_main(void){
	game_start();
	while(!ir_beam_blocked()){
	}
	
	game_end();
}



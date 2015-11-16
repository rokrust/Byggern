#include <stdint.h>
#include "../Drivers/Communication/can/mcp/mcp.h"
#include "../Drivers/Communication/can/mcp/mcp_defines.h"
#include "../Drivers/Communication/can/can.h"
#include "game.h"


game_score score = new_score(0, 0, 0, 0);

void update_game_score(){
	uint8_t newState = 0;
	score.sum += ir_value;
	(score.antallMaalinger)++;
	
	if(score.antallMaalinger == 20){
		score.sum = score.sum/score.antallMaalinger;
		score.antallMaalinger = 1;
		if(score.sum < 100){
			newState = 1;
		}
		else{
			score.boolState = 0;
			newState = 0;
		}
	}
	
	//Registering point
	if(!score.boolState && newState){
		score.boolState = 1;
		(score.score)++;
		
	}
	
}

int game_beam_blocked(uint8_t ir_value){
	uint8_t newState = 0;
	score.sum += ir_value;
	(score.antallMaalinger)++;
	
	if(score.antallMaalinger == 20){
		score.sum = score.sum/score.antallMaalinger;
		score.antallMaalinger = 1;
		if(score.sum < 100){
			newState = 1;
		}
		else{
			score.boolState = 0;
			newState = 0;
		}
	}
	
	//Registering point
	if(!score.boolState && newState){
		score.boolState = 1;
		(score.score)++;
		
	}

}


game_score new_score(uint8_t antallMaalinger, uint16_t sum, uint8_t boolState, uint8_t val){
	game_score score;
	score.antallMaalinger = antallMaalinger;
	score.sum = sum;
	score.boolState = boolState;
	score.score = val;
	
	return score;
}
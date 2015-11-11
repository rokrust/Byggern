#include <stdint.h>
#include "mcp.h"
#include "mcp_defines.h"
#include "can.h"
#include "game.h"
#include "adc.h"

void update_game_score(game_score *score){
	uint8_t newState = 0;
	score->sum += adc_read(0);
	(score->antallMaalinger)++;
	
	if(score->antallMaalinger == 20){
		score->sum = score->sum/score->antallMaalinger;
		score->antallMaalinger = 1;
		if(score->sum < 100){
			newState = 1;
		}
		else{
			score->boolState = 0;
			newState = 0;
		}
	}
	
	//Registering point
	if(!score->boolState && newState){
		score->boolState = 1;
		(score->score)++;
	}
	
	//printf("Score: %d\n", score->score);
	
	can_message msg;
	msg = new_can_message(MCP_GAME_SCORE_MESSAGE, 1, &score->score);
	can_write(&msg, MCP_TXB0CTRL);
}

game_score new_score(uint8_t antallMaalinger, uint16_t sum, uint8_t boolState, uint8_t val){
	game_score score;
	score.antallMaalinger = antallMaalinger;
	score.sum = sum;
	score.boolState = boolState;
	score.score = val;
	
	return score;
}
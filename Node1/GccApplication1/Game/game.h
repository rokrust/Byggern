typedef struct game_score game_score;

struct game_score{
	uint8_t antallMaalinger;
	uint16_t sum;
	
	uint8_t boolState;
	
	uint8_t score;
	
};

void update_game_score(uint8_t ir_value);
game_score new_score(uint8_t antallMaalinger, uint16_t sum, uint8_t boolState, uint8_t val);
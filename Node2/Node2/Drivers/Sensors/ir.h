#ifndef _IR_H_
#define _IR_H_

#include <stdint.h>

typedef struct ir_state ir_state;
struct ir_state{
	uint8_t samples;
	uint16_t sum;
	
	uint8_t beam_blocked;
	uint8_t new_state;
	
};

void ir_init(void);
uint8_t ir_beam_blocked(uint8_t ir_value);
ir_state new_ir_state(uint8_t samples, uint16_t sum, uint8_t beam_blocked, uint8_t new_state);
#endif
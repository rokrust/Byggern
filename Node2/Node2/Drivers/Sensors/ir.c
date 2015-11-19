#include "ir.h"
#include "../can/can.h"
#include "../adc/adc.h"
ir_state ir;

void ir_init(void){
	ir = new_ir_state(0, 0, 0, 0);
}

uint8_t ir_beam_blocked(uint8_t ir_value){
	//Used to determine if the beam was already blocked
	ir.sum += ir_value;
	(ir.samples)++;
	
	//Check state every 20 measures by calculating mean
	if(ir.samples == 20){
		ir.sum = ir.sum/ir.samples;
		ir.samples = 1;
		
		//IR beam blocked for more than 20 samples
		if(ir.sum < 100){
			ir.new_state = 1;
		}
		
		else{
			ir.beam_blocked = 0;
			ir.new_state = 0;
		}
	}

	//Blocked now, but not blocked the last 20 samples
	if(!ir.beam_blocked && ir.new_state){
		ir.beam_blocked = 1;
		ir.new_state = 1;
		printf("Game lost.");
		can_transmit_ir_value(ir.beam_blocked);
		
	}
	return 0;

}


ir_state new_ir_state(uint8_t samples, uint16_t sum, uint8_t beam_blocked, uint8_t new_state){
	ir_state ir;
	ir.samples = samples;
	ir.sum = sum;
	ir.beam_blocked = beam_blocked;
	
	return ir;
}
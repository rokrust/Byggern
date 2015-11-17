#include "../Communication/can/mcp/mcp.h"
#include "../Communication/can/mcp/mcp_defines.h"
#include "../Communication/can/can.h"
#include "../../Game/game.h"
#include "ir.h"

ir_state ir;

void ir_init(void){
	ir = new_ir_state(0, 0, 0);
}

uint8_t ir_beam_blocked(uint8_t ir_value){
	uint8_t newState = 0;
	ir.sum += ir_value;
	(ir.samples)++;
	
	//Check state every 20 measures
	if(ir.samples == 20){
		ir.sum = ir.sum/ir.samples;
		ir.samples = 1;
		
		//Beam blocked continously
		if(ir.sum < 100){
			newState = 1;
		}
		else{
			ir.beam_blocked = 0;
			newState = 0;
		}
	}
	
	//IR beam blocked for more than 20 samples
	if(!ir.beam_blocked && newState){
		ir.beam_blocked = 1;
		return 1;
	}
	return 0;

}


ir_state new_ir_state(uint8_t samples, uint16_t sum, uint8_t beam_blocked){
	ir_state ir;
	ir.samples = samples;
	ir.sum = sum;
	ir.beam_blocked = beam_blocked;
	
	return ir;
}
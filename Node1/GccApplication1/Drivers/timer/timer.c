#include "../Sensors/ir.h"
#include "../../Game/game.h"
#include "../MultifunctionCard/joystick/joy.h"
#include "../Communication/can/can.h"
#include "../Sensors/ir.h"
#include "../Communication/can/mcp/mcp_defines.h"
#include "../MultifunctionCard/oled/oled.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


void timer_init() {
	TCCR3A &= ~(1<<WGM30);
	TCCR3A &= ~(1<<WGM31);
	TCCR3B |=  (1<<WGM32);
	TCCR3B &= ~(1<<WGM33);
	
	OCR3A = 7*68; //68 for 70Hz, 4760 for 1Hz
	
	//enable interrupt on OCR3A compare
	ETIMSK |= (1<<OCIE3A);
	TIFR |= (1<<ICF1);
	
	sei();

}

void timer_enable(){
	//CS3{2:0}   = 101  prescaler set to 1024
	TCCR3B |=  (1<<CS32) | (1<<CS30);
	TCCR3B &= ~(1<<CS31);
}

void timer_disable(){
	TCCR3B &= ~((1<<CS30) | (1<<CS31) | (1<<CS32));
}

void timer_toggle(){
	if((TCCR3B & 0x07) == 0x00){
		timer_enable();
	} 
	else{
		timer_disable();
	}
}

ISR(TIMER3_COMPA_vect){
	can_message msg = new_can_message(0, 0, NULL);
	if(can_pollInterrupt()){
		msg = can_read();
	}
	
	if(msg.id == MCP_IR_MESSAGE){
		game_lost = msg.data[0];
	}
	
	if (!game_lost){	
		game_track_score();
	}
	

	oled_toggle_party_mode();
	
	
	can_send_joystick_message();
	can_send_slider_message();
	can_send_button_message();
	
}
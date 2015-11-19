#include "../../../Config/config.h"
#include "joy.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

Joystick joy;

void joy_init(){
	joy_calibrate();
	DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB2));
	PORTB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
	DDRD &= ~(1 << DDD2);
	PORTD |= (1 << PD2);
}
Joystick joy_get_joystick(void){
	return joy;
}
Joystick joy_toPercent(){
	joy_read_joystick();
	int8_t x = (int8_t)((100.0*(joy.x - joy.offset_x))/((float)255-joy.offset_x));
	int8_t y = (int8_t)((100.0*(joy.y - joy.offset_y))/((float)255-joy.offset_y));
	
	return joy_new_joystick(x, y, joy.offset_x, joy.offset_y);
}
void joy_calibrate(){
	joy.offset_x = joy_read_x();
	joy.offset_y = joy_read_y();
}
void joy_read_joystick(){
	joy.x = joy_read_x();
	joy.y = joy_read_y();

}
Joystick joy_new_joystick(int x, int y, uint8_t offset_x, uint8_t offset_y){
	Joystick j;
	j.x = x;
	j.y = y;
	j.offset_x = offset_x;
	j.offset_y = offset_y;
	
	return j;
}
direction joy_get_direction(){
	Joystick j = joy_toPercent();
	if(j.x >= 90){
		return RIGHT;
	}else if(j.x <= -90){
		return LEFT;
	}else if(j.y >= 90){
		return UP;
	}else if(j.y <= -90){
		return DOWN;
	}else{
		return NEUTRAL;
	}		
}

uint8_t joy_read_x(void){
	*adc_data = 0x04;
	_delay_us(60);
	return (*adc_data);
}
uint8_t joy_read_y(void){
	*adc_data = 0x05;
	_delay_us(60);
	return (*adc_data);
}
uint8_t joy_read_right_slider(void){				//Kan ha blandet left og right
	volatile char *ext_joy = (char *) adc_data;	//MUX Address
	ext_joy[0] = 0x06;							//Enable CH3
	_delay_us(40);
	return (uint8_t)(ext_joy[0]);
}
uint8_t joy_read_left_slider(void){
	volatile char *ext_joy = (char *) adc_data;	//MUX Address
	ext_joy[0] = 0x07;							//Enable CH4
	_delay_us(40);
	return (uint8_t)(ext_joy[0]);
}
int joy_read_left_button(void){
	return (PINB & (1 << PINB1)) >> PINB1;
}
int joy_read_right_button(void){
	return (PINB & (1 << PINB0)) >> PINB0;
}
int joy_read_toggle_button(void){
	return PINB;
}

#include "config.h"
#include "joy.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void JOY_init(Joystick* joy){
	//MCUCR |=
	//GICR |= (1 << INT0);		<-- Interrupts
	//sei();
	
	JOY_calibrate(joy);
	DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB2));
	PORTB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
	DDRD &= ~(1 << DDD2);
	PORTD |= (1 << PD2);
}
void JOY_toPercent(Joystick* joy){
	joy->x = (int8_t)((100.0*(JOY_read_x() - joy->offset_x))/((float)255-joy->offset_x));
	joy->y = (int8_t)((100.0*(JOY_read_y() - joy->offset_y))/((float)255-joy->offset_y));
}
void JOY_calibrate(Joystick* joy){
	joy->offset_x = JOY_read_x();
	printf("X zero: %d\n", joy->offset_x);
	joy->offset_y = JOY_read_y();
	printf("Y zero: %d\n", joy->offset_y);
}
void JOY_read_joystick(Joystick* joy){
	joy->x = JOY_read_x();
	joy->y = JOY_read_y();

}
Joystick JOY_new_joystick(uint8_t x, uint8_t y, uint8_t offset_x, uint8_t offset_y){
	Joystick j;
	j.x = x;
	j.y = y;
	j.offset_x = offset_x;
	j.offset_y = offset_y;
	
	return j;
}
direction JOY_get_direction(Joystick joy){
	JOY_toPercent(&joy);
	if(joy.x >= 90){
		return RIGHT;
	}else if(joy.x <= -90){
		return LEFT;
	}else if(joy.y >= 90){
		return UP;
	}else if(joy.y <= -90){
		return DOWN;
	}else{
		return NEUTRAL;
	}		
}

uint8_t JOY_read_x(void){
	*adc_data = 0x04;
	_delay_us(60);
	return (*adc_data);
}
uint8_t JOY_read_y(void){
	*adc_data = 0x05;
	_delay_us(60);
	return (*adc_data);
}
uint8_t JOY_read_right_slider(void){				//Kan ha blandet left og right
	volatile char *ext_joy = (char *) adc_data;	//MUX Address
	ext_joy[0] = 0x06;							//Enable CH3
	_delay_us(40);
	return (uint8_t)(ext_joy[0]);
}
uint8_t JOY_read_left_slider(void){
	volatile char *ext_joy = (char *) adc_data;	//MUX Address
	ext_joy[0] = 0x07;							//Enable CH4
	_delay_us(40);
	return (uint8_t)(ext_joy[0]);
}
int JOY_read_left_button(void){
	return (PINB & (1 << PINB1)) >> PINB1;
}
int JOY_read_right_button(void){
	return (PINB & (1 << PINB0)) >> PINB0;
}
int JOY_read_toggle_button(void){
	return PINB;
}
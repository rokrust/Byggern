#include "../../Config/config.h"
#include "../dac/dac.h"
#include "motor_driver.h"

#include <avr/io.h>
#include <util/delay.h>


#define test_bit( reg, bit ) (reg & (1 << bit))
void motor_init(void) {
	DDRF |= (1<<MOTOR_DIR) | (1<<MOTOR_EN) | (1<<MOTOR_SEL) | (1<<MOTOR_RST) | (1<<MOTOR_OE);
	DDRK = 0;
	PORTF |= (1 << MOTOR_EN);
	motor_encoder_reset();
	
}

int16_t motor_encoder_read(void){
	uint16_t data;
	
	//Enable output and send high byte on PORT K
	PORTF &=	~((1 << MOTOR_OE) |
			(1 << MOTOR_SEL));
	_delay_us(20);
	data = (reverse(PINK) << 8);
	
	//Send low byte on PORT K
	PORTF |= (1 << MOTOR_SEL);
	_delay_us(20);
	data += reverse(PINK);
	
	//Disable output
	PORTF |= (1 << MOTOR_OE);
	
	return data;
}

void motor_encoder_reset(){
	PORTF &= ~(1 << MOTOR_RST);
	_delay_us(20);
	PORTF |= (1 << MOTOR_RST);
}

void motor_speed(int16_t speed){
	if(speed > 0){
		motor_direction(right);
	}
	else{
		motor_direction(left);
		speed = abs(speed);
	}
	
	if(speed > max_speed){
		speed = max_speed;
	}
	
	dac_write(0, speed);
}

void motor_direction(motorDirection dir){
	if(dir){
		PORTF &= ~(1 << MOTOR_DIR);
	}
	else{
		PORTF |= (1 << MOTOR_DIR);
	}
}

void motor_test(void){
	motor_direction(right);
	_delay_ms(500);
	motor_direction(left);
	_delay_ms(500);

}

uint8_t reverse(uint8_t x){
	x = (((x & 0xaa) >> 1) | ((x & 0x55) << 1));
	x = (((x & 0xcc) >> 2) | ((x & 0x33) << 2));
	x = (((x & 0xf0) >> 4) | ((x & 0x0f) << 4));
	return x;
}




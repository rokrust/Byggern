#include "pwm.h"
#include "../../Config/config.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

void pwm_init(void){
	DDRE |= (1 << PE3);
	
	TCCR3A &= ~((1 << WGM30) | (1 << COM3A0));
	TCCR3A |= ((1 << WGM31) | (1 << COM3A1));
	TCCR3B |= (1 << WGM32) | (1 << WGM33) | (1 << CS32) | (1 << CS30);
	TCCR3B &= ~(1 << CS31);
	
	ICR3 = 312;
}

void pwm_set_value(unsigned int val){
	OCR3A = val;
}

void pwm_set_servo(unsigned int val){
	//Step length between 18 and 19.
	val = (val*19)/255 + servo_min;
	
	//Fixes seizures in the servo
	if((val <= (servo_mid + 1)) && (val >= (servo_mid - 1))){
		pwm_set_value(servo_mid);
	}
	
	else if((val >= servo_min) && (val <= servo_max)){
		pwm_set_value(val);
	}
	else{
		pwm_set_value(servo_mid);
	}
	
}


void pwm_test(void){
	pwm_set_value(140);
	_delay_ms(20);
	pwm_set_value(50);
	_delay_ms(20);
	pwm_set_value(200);
	_delay_ms(20);
	
}
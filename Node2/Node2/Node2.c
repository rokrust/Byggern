/*
 * Node2.c
 *
 * Created: 23.10.2015 14:22:15
 *  Author: haakome
 */ 

#include "config.h"

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

 
#include "joy.h"
#include "pwm.h"
#include "usart.h"
#include "spi.h"
#include "mcp.h"
#include "mcp_defines.h"
#include "can.h"
#include "game.h"
#include "adc.h"
#include "motor_driver.h"
#include "dac.h"
#include "motor_controller.h"
#include "pid.h"

float max_encoder_value;

int main(void)
{
	sei();
	USART_Init(MYUBRR);
	fdevopen((int (*)(char,  struct __file *))USART_Transmit, (int (*)(struct __file *))USART_Receive);
	printf("UART setup done\n");
	can_init();
	printf("CAN setup done\n");
	pwm_init();
	printf("PWM setup done\n");
	adc_init();
	printf("ADC setup done\n");
	motor_init();
	printf("Motor setup done\n");
	dac_initialize(0b111);
	printf("DAC setup done\n");
	printf("Initialization done\n");
	
	DDRA |=(1<<PA2); //for solenoid
	//Joystick joy;
	can_message msg = {0};
	game_score score = new_score(0, 0, 0, 0);
	max_encoder_value = controller_init();
	
	printf("max_encoder_value: %.3f\n", max_encoder_value);
	
	
	pid_init(-1, -2, -0.0);
	
	
	//motor_speed(100);
	while(1)
	{
		if(can_pollInterrupt()){
			msg = can_read();
		}
		
		//Kanskje bruke en switch
		can_handle_joystick_message(msg);
		can_handle_score_message(msg);
		can_handle_slider_message(msg); //FJERN GLOBAL VARIABEL


		

		
		//printf("Speed: %d\n", speed);
		//motor_speed(speed);
		
		_delay_ms(10);
	}
	
	return 1;
}
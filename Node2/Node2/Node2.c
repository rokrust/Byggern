/*
 * Node2.c
 *
 * Created: 23.10.2015 14:22:15
 *  Author: haakome
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>


#include "config.h"
#include "joy.h"
#include "pwm.h"
#include "usart.h"
#include "spi.h"
#include "mcp.h"
#include "mcp_defines.h"
#include "can.h"
#include "game.h"
#include "adc.h"

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
	printf("Initialization done\n");
	
	DDRA |=(1<<PA2); //for solenoid
	Joystick joy;
	can_message msg;
	game_score score;
	score.antallMaalinger = 0;
	score.boolState = 0;
	score.score = 0;
	score.sum = 0;
	
	while(1)
	{
		printf("IR: %d\n", adc_read(0));
		if(can_pollInterrupt()){
			msg = can_read();
			
		}
		if(msg.id == MCP_JOYSTICK_MESSAGE){
			joy = JOY_new_joystick(msg.data[0], msg.data[1], msg.data[2], msg.data[3]);
			pwm_set_servo(joy.x);
		}
		
		if(msg.id == MCP_BUTTON_PRESS){
			printf("Button pressed");
			if(msg.data[0]){
				PORTA |= (1<<PA2); //stop relay
			}
			else{
				PORTA &= ~(1<<PA2); //activate relay
			}
		}
		
		update_game_score(&score);
	}
	
	return 1;
}
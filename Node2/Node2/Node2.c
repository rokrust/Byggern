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

int main(void)
{
	sei();
	USART_Init(MYUBRR);
	fdevopen((int (*)(char,  struct __file *))USART_Transmit, (int (*)(struct __file *))USART_Receive);
	printf("UART setup done\n");
	can_init();
	printf("Initialization done\n");
	pwm_init();
	
	Joystick joy;
	can_message msg;
	while(1)
	{
		//pwm_test();
		
		
		
		if(can_pollInterrupt()){
			msg = can_read();
			
		}
		if(msg.id == MCP_JOYSTICK_MESSAGE){
			joy = JOY_new_joystick(msg.data[0], msg.data[1], msg.data[2], msg.data[3]);
			pwm_set_servo(joy.x);
		}
		
	}
	
	return 1;
}
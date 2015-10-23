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


	uint8_t str[5] = {'h', 'a', 'l', 'l', 'a'};
	can_message msg = new_can_message(0x00, 5, str);
	while(1)
	{
		can_write(&msg, MCP_TXB0CTRL);
		can_print(can_read());

	}
	
	return 1;
}
/*
 * skynetAlpha.c
 *
 * Created: 28.08.2015 10:35:16
 *  Author: haakome
 */
 
// git shit : clone, *dra inn i mappen*, add, commit, push


#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h> 

#include "config.h"
 
#include "joy.h"
#include "usart.h"
#include "oled.h"
#include "SRAM.h"
#include "interface.h"
#include "spi.h"
#include "mcp.h"
#include "can.h"
#include "mcp_defines.h"


int main(void)
{
	Joystick joy_position;
	volatile int select_pos = 1;
	Menu *current_menu;
	
	USART_Init(MYUBRR);
	fdevopen((int (*)(char,  struct __file *))USART_Transmit, (int (*)(struct __file *))USART_Receive);
	printf("UART setup done\n");
	SRAM_init();
	printf("SRAM setup done\n");
	JOY_init(&joy_position);
	printf("JOY setup done\n");
	oled_init();
	can_init();
	printf("Initialization done\n");
	current_menu = interface_init();

	//putte denne i init
	interface_print(current_menu);
	
	can_message msg;
	//mcp_write(CANSTAT)
    while(1)
        {	
			JOY_read_joystick(&joy_position);
			uint8_t joy_data[4] = {joy_position.x, joy_position.y, joy_position.offset_y, joy_position.offset_y};
			msg = new_can_message(MCP_JOYSTICK_MESSAGE, 4, joy_data);
			can_write(&msg, MCP_TXB0CTRL);
			//interface_select(joy_position, &select_pos, &current_menu);
			
		}
		
	return 1;
}
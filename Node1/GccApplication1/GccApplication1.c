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

#include "Config/config.h"
#include "Drivers/MultifunctionCard/joystick/joy.h"
#include "Drivers/MultifunctionCard/oled/oled.h"
#include "Drivers/Memory/SRAM.h"
#include "Drivers/Communication/usart/usart.h"
#include "Drivers/Communication/spi/spi.h"
#include "Drivers/Communication/can/can.h"
#include "Drivers/Communication/can/mcp/mcp.h"
#include "Drivers/Communication/can/mcp/mcp_defines.h"
#include "Interface/interface.h"

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
	
	uint8_t ir_value = 0;
	can_message msg;
	
    while(1)
        {	
			JOY_read_joystick(&joy_position);
			can_send_joystick_message(joy_position);
			interface_select(joy_position, &select_pos, &current_menu);
			
			if(can_pollInterrupt()){
				msg = can_read();
				
			}
			
			if(msg.id == MCP_IR_MESSAGE){
				ir_value = msg.data[0];
			}
			
			uint8_t msg_data = JOY_read_left_slider();
			msg = new_can_message(MCP_SLIDER_MESSAGE, 1, &msg_data);
			can_write(&msg, MCP_TXB0CTRL);
			
			msg_data = JOY_read_right_button();
			msg = new_can_message(MCP_SOLENOID_MESSAGE, 1, &msg_data);
			can_write(&msg, MCP_TXB0CTRL);
			
		}
		
	return 1;
}
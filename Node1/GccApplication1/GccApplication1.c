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
#include "Drivers/timer/timer.h"
#include "Game/game.h"
#include "Game/highscore.h"
#include "Interface/interface.h"

int main(void)
{
	
	USART_Init(MYUBRR);
	fdevopen((int (*)(char,  struct __file *))USART_Transmit, (int (*)(struct __file *))USART_Receive);
	printf("UART setup done\n");
	SRAM_init();
	printf("SRAM setup done\n");
	joy_init();
	printf("joy setup done\n");
	oled_init();
	can_init();
	timer_init();
	ir_init();
	highscore_init();
	game_init(10);
	printf("Initialization done\n");
	interface_init();
	highscore_resetAll();
    while(1)
        {	
			interface_select();
		}
		
	return 1;
}
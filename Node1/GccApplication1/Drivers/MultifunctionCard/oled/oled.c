/*****************************
Initiate party mode

oled_control_assign(0xa6);
_delay_ms(100);
oled_control_assign(0xa7);
_delay_ms(100);
*****************************/


#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>

#include "oled.h"
#include "../../../Fonts/font.h"
#include "../joystick/joy.h"
static FILE oled_stdout = FDEV_SETUP_STREAM(oled_write, NULL, _FDEV_SETUP_WRITE);

int is_party_toggle;

void oled_init(){
	oled_control_assign(0xae);    //display off
	oled_control_assign(0xa1);    //segment remap
	oled_control_assign(0xda);    //common pads hardware: alternative
	oled_control_assign(0x12);
	oled_control_assign(0xc8);    //common output scan direction:com63~com0
	oled_control_assign(0xa8);    //multiplex ration mode:63
	oled_control_assign(0x3f);
	oled_control_assign(0xd5);    //display divide ratio/osc. freq. mode
	oled_control_assign(0x80);
	oled_control_assign(0x81);    //contrast control
	oled_control_assign(0x50);
	oled_control_assign(0xd9);    //set pre-charge period
	oled_control_assign(0x21);
	oled_control_assign(0x20);    //Set Memory Addressing Mode
	oled_control_assign(0x02);
	oled_control_assign(0xdb);    //VCOM deselect level mode
	oled_control_assign(0x30);
	oled_control_assign(0xad);    //master configuration
	oled_control_assign(0x00);
	oled_control_assign(0xa4);    //out follows RAM content
	oled_control_assign(0xa6);    //set normal display
	oled_control_assign(0xaf);    // display on
	oled_clear_screen();
	
	is_party_toggle = 0;
}



void oled_control_assign(uint8_t verdi){
	*oled_control = verdi;
}
void oled_write_selector(){
	for(uint8_t col = 0; col < font_width; col++){
		*oled_data = selector[col];
		
	}
}
void oled_write(uint8_t character){	
	for(uint8_t col = 0; col < font_width; col++){
		*oled_data = pgm_read_byte(&font[character-' '][col]);
		
	}
}

void oled_set_start_col(int start_col){
	oled_control_assign(0x21);
	oled_control_assign(start_col);
	oled_control_assign(0xFF);
}

void oled_write_string(char* str, int line_nbr){
	oled_control_assign(0xb0 + line_nbr);
	for(int i = 0; str[i] != '\0'; i++){
		oled_write(str[i]);
	}
}

void oled_clear_screen(){
	oled_set_start_col(0);
	for(int page = 0; page < 8; page++){
		oled_control_assign(0xB0 + page);
		for(int i = 0; i < 128; ++i){
			*oled_data = 0x00;
		}
	}
}

void oled_set_write_position(int row, int col){
	oled_set_start_col(col);
	oled_control_assign(0xb0 + row);
}

void oled_toggle_party_mode(void){
	//Invert screen
	if(is_party_toggle){
		oled_control_assign(0xa6);
		is_party_toggle=0;	
	}
	else{
		oled_control_assign(0xa7);
		is_party_toggle=1;		
	}
}

void oled_printf(char* fmt, ...){
	va_list v;
	va_start(v, fmt);
	vfprintf(&oled_stdout, fmt, v);
	va_end(v);
}

void oled_set_brightness(){
	oled_clear_screen();
	oled_set_write_position(2, 0);
	oled_printf("Use the left");
	oled_set_write_position(3, 0);
	oled_printf("slider to");
	oled_set_write_position(4, 0);
	oled_printf("set brightness");
	while(joy_get_direction() != LEFT){
		oled_control_assign(0x81);
		oled_control_assign(joy_read_left_slider());
	}
}
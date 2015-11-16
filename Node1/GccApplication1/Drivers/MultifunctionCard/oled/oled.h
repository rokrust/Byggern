#ifndef _OLED_
#define _OLED_
#define oled_control ((volatile char*)0x1000)
#define oled_data  ((volatile char*)0x1200)
#include <avr/pgmspace.h>


void oled_printf(char* fmt, ...);
void oled_init(void);
void oled_control_assign(uint8_t verdi);
void oled_write(uint8_t character);
void oled_write_string(char* str, int line_nbr);
void oled_write_selector(void);
void oled_set_start_col(int start_col);
void oled_clear_screen(void);
void oled_initiate_party_mode(void);
#endif
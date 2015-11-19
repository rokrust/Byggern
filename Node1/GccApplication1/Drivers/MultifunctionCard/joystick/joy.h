#ifndef _JOY_H_
#define _JOY_H_
#include <stdint.h>

#define adc_data ((volatile char*)0x1400)
typedef struct Joystick Joystick;

struct Joystick {
	volatile int x;
	volatile int y;
	
	int offset_x;
	int offset_y;
};
typedef enum {NEUTRAL, UP, DOWN, LEFT, RIGHT} direction;

void joy_init();
Joystick joy_get_joystick();
uint8_t joy_read_x(void);
uint8_t joy_read_y(void);
uint8_t joy_read_left_slider(void);
uint8_t joy_read_rigth_slider(void);
int joy_read_left_button(void);
int joy_read_right_button(void);
int joy_read_toggle_button(void);
Joystick joy_toPercent();

void joy_read_joystick();

void joy_calibrate();

direction joy_get_direction();
Joystick joy_new_joystick(int x, int y, uint8_t offset_x, uint8_t offset_y);
#endif
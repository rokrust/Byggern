#ifndef _JOY_
#define _JOY_
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

void JOY_init(Joystick* joy);
uint8_t JOY_read_x(void);
uint8_t JOY_read_y(void);
uint8_t JOY_read_left_slider(void);
uint8_t JOY_read_rigth_slider(void);
int JOY_read_left_button(void);
int JOY_read_right_button(void);
int JOY_read_toggle_button(void);
void JOY_toPercent(Joystick* joy);

void JOY_read_joystick(Joystick* joy);

void JOY_calibrate(Joystick* joy);

direction JOY_get_direction(Joystick joy);
Joystick JOY_new_joystick(uint8_t x, uint8_t y, uint8_t offset_x, uint8_t offset_y);
#endif
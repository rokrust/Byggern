#ifndef _JOY_
#define _JOY_
#include <stdint.h>

typedef struct Joystick Joystick;

struct Joystick {
	volatile int x;
	volatile int y;
	
	int offset_x;
	int offset_y;
};
typedef enum {NEUTRAL, UP, DOWN, LEFT, RIGHT} direction;

void JOY_toPercent(Joystick* joy);
direction JOY_get_direction(Joystick joy);
Joystick JOY_new_joystick(uint8_t x, uint8_t y, uint8_t offset_x, uint8_t offset_y);
#endif
#include <stdint.h>
#include "joy.h"

void JOY_toPercent(Joystick *joy){
	joy->x = (int8_t)((100.0*(JOY_read_x() - joy->offset_x))/((float)255-joy->offset_x));
	joy->y = (int8_t)((100.0*(JOY_read_y() - joy->offset_y))/((float)255-joy->offset_y));
}
direction JOY_get_direction(Joystick joy){
	JOY_toPercent(&joy);
	if(joy.x >= 90){
		return RIGHT;
		}else if(joy.x <= -90){
		return LEFT;
		}else if(joy.y >= 90){
		return UP;
		}else if(joy.y <= -90){
		return DOWN;
		}else{
		return NEUTRAL;
	}
}

Joystick JOY_new_joystick(uint8_t x, uint8_t y, uint8_t offset_x, uint8_t offset_y){
	Joystick j;
	j.x = x;
	j.y = y;
	j.offset_x = offset_x;
	j.offset_y = offset_y;
	
	return j;
}
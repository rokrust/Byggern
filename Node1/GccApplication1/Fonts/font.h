/*
 * font.h
 *
 * Created: 18.09.2015 16:33:19
 *  Author: oyvinaro
 */ 



#ifndef FONT_H_
#define FONT_H_

#include <avr/pgmspace.h>

extern const unsigned char PROGMEM font[95][8];
const unsigned char selector[8];

#define font_width 8


#endif /* FONT_H_ */

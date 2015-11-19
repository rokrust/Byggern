#include "solenoid.h"
#include <avr/io.h>

void solenoid_init(){
	DDRA |=(1<<PA2);
	PORTA |= (1 << PA2);	
}
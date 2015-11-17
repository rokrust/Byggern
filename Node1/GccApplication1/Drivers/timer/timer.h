#include "../Sensors/ir.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void timer_init();

void timer_enable();

void timer_disable();

void timer_toggle();
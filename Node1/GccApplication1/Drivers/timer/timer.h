#ifndef _TIMER_H_
#define _TIMER_H_
#include "../Sensors/ir.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void timer_init();

void timer_enable();

void timer_disable();

void timer_toggle();

int timer_get_partyCounter();
#endif
#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_
#include "../Drivers/Memory/eeprom.h"
#include <avr/io.h>
//Skal lagre syv highscores
#define HIGHSCORE_START_ADDR 0
#define HIGHSCORE_STOP_ADDR 7
#define NUMBER_OF_HIGHSCORES 7

void highscore_init(void);
int highscore_update(uint8_t score);
void highscore_resetAll(void);
void highscore_print(void);
#endif
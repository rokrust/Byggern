#include "../Interface/interface.h"
#include "highscore.h"
#include <stdio.h>

uint8_t highscores[7];

//Save highscores to array
void highscore_init(){
	for(int i = 0; i < NUMBER_OF_HIGHSCORES; i++){
		highscores[i] = EEPROM_read(HIGHSCORE_START_ADDR + i);
	}
}
	
void highscore_update(uint8_t score){
	for(int i = 0; i < NUMBER_OF_HIGHSCORES; i++){
		if(score > highscores[i]){
			for(int j = NUMBER_OF_HIGHSCORES - 1; j == i; j--){
				highscores[j - 1] = highscores[j];
				EEPROM_write(HIGHSCORE_START_ADDR + j, highscores[j]);
			}
			highscores[i] = score;
			EEPROM_write(HIGHSCORE_START_ADDR + i, highscores[i]);
			break;
		}
	}
}

void highscore_resetAll(void){
	for (int i = 0; i < NUMBER_OF_HIGHSCORES; i++){
		EEPROM_write(HIGHSCORE_START_ADDR + i, 0);
	}
}

void highscore_print(void){
	oled_clear_screen();
	oled_printf("Highscores!", 0);
	
	for(int i = 0; i < NUMBER_OF_HIGHSCORES; i++){
		oled_printf("%d.  %d", i + 1, highscores[i]);
	}
}
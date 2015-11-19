#include "../Interface/interface.h"
#include "highscore.h"
#include <util/delay.h>
#include <stdio.h>

uint8_t highscores[7];

//Save highscores to array
void highscore_init(){
	for(int i = 0; i < NUMBER_OF_HIGHSCORES; i++){
		highscores[i] = EEPROM_read(HIGHSCORE_START_ADDR + i);
	}
}
	
int highscore_update(uint8_t score){
	for(int i = 0; i < NUMBER_OF_HIGHSCORES; i++){
		if(score > highscores[i]){
			for(int j = NUMBER_OF_HIGHSCORES - 1; j > i - 1; j--){
				highscores[j] = highscores[j - 1];
				EEPROM_write(HIGHSCORE_START_ADDR + j, highscores[j]);
			}
			highscores[i] = score;
			EEPROM_write(HIGHSCORE_START_ADDR + i, highscores[i]);
			return i;
		}
	}
	return 0;
}

void highscore_resetAll(void){
	for (int i = 0; i < NUMBER_OF_HIGHSCORES; i++){
		EEPROM_write(HIGHSCORE_START_ADDR + i, 0);
		highscores[i] = 0;
	}
	
}

void highscore_print(void){
	oled_clear_screen();
	oled_set_write_position(0, 0);
	oled_printf("Highscores!", 0);
	for(int i = 0; i < NUMBER_OF_HIGHSCORES; i++){
		oled_set_write_position(i + 1, 16);
		oled_printf("%d.  %d", i + 1, highscores[i]);
	}
	while(joy_get_direction() != LEFT){
		_delay_us(10);
	}
	interface_print(interface_get_current_menu());
}
#ifndef _GAME_H_
#define _GAME_H_
#include <stdint.h>
void game_init(uint8_t speed);
void game_start(void);
void game_track_score();
void game_end(void);
void game_reset_score(void);
void game_main(void);
void game_hard_difficulty(void);
void game_medium_difficulty(void);
void game_impossible_difficulty(void);
void game_introMessage(void);
void game_postGameMessage(int rank);
int game_lost;



#endif
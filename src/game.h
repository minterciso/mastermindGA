#ifndef __GAME_H
#define __GAME_H

#include "consts.h"

typedef struct{
	unsigned int secret[QTD_ANSWER];
	unsigned int guesses[MAX_MOVES][QTD_ANSWER];
	unsigned int results[MAX_MOVES][QTD_ANSWER];
	unsigned int guess;
} game_board;

typedef enum game_error{
	game_success,
	invalid_board,
	invalid_peg_results,
	invalid_guess,
	maximum_guess
} game_error_t;

void print_game_error(game_error_t error);

game_error_t initialize_board(game_board *board);
game_error_t add_guess(game_board *board, unsigned int guess[QTD_ANSWER], unsigned int peg_results[QTD_ANSWER]);



#endif //__GAME_H

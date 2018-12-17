#ifndef __GAME_H
#define __GAME_H

#include <stdlib.h>
#include "consts.h"

#define GAME_CHECK(call)							\
{										\
	game_error_t error=call;						\
	if(error != game_success)						\
	{									\
		fprintf(stderr, "Error: %s: %d\n",__FILE__,__LINE__);		\
		fprintf(stderr, "Code: %d\n", error);				\
		print_game_error(error);					\
		exit(EXIT_FAILURE);						\
	}									\
}

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
game_error_t reset_guesses(game_board *board);



#endif //__GAME_H

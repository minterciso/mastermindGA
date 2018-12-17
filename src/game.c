#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "consts.h"
#include "game.h"

void print_game_error(game_error_t error)
{
	switch(error){
		case game_success: break;
		case invalid_board: fprintf(stderr, "Invalid Board.\n");
		case invalid_peg_results: fprintf(stderr, "Invalid peg results variabl.\n");
		case invalid_guess: fprintf(stderr, "Invalid guess.\n");
		case maximum_guess: fprintf(stderr, "Maximum number of guesses reached.\n");
		default: break;
	}
}

game_error_t initialize_board(game_board *board)
{
	int i,j;
	if(board == NULL)
		return invalid_board;
	srand(time(NULL));
	board->guess = 0;
	for(i=0;i<QTD_ANSWER;i++)
		board->secret[i] = rand() % QTD_PEGS;
	for(i=0;i<MAX_MOVES;i++)
	{
		for(j=0;j<QTD_ANSWER;j++)
		{
			board->guesses[i][j]=0;
			board->results[i][j]=0;
		}
	}
	return game_success;
}

game_error_t add_guess(game_board *board, unsigned int guess[QTD_ANSWER], unsigned int peg_results[QTD_ANSWER])
{
	int i;
	if(board == NULL)
		return invalid_board;
	if(peg_results == NULL)
		return invalid_peg_results;
	if(board->guess == MAX_MOVES)
		return maximum_guess;
	// Zero the peg results
	memset(peg_results,0,sizeof(unsigned int)*QTD_ANSWER);

	// Create white peg results (black peg is the secret from the board)
	unsigned int white_peg_results[QTD_PEGS];
	memset(white_peg_results, 0, sizeof(white_peg_results));
	int peg_color = -1;
	for(i=0;i<QTD_ANSWER;i++)
	{
		peg_color = board->secret[i];
		white_peg_results[peg_color]++;
	}
	int peg_result_idx = 0;
	memcpy(board->guesses[board->guess], guess, sizeof(unsigned int)*QTD_ANSWER);
	// Now search for black pegs and remove them from the guess
	for(i=0;i<QTD_ANSWER;i++)
	{
		peg_color = guess[i];
		if(peg_color > QTD_PEGS)
			return invalid_guess;
		if(peg_color == board->secret[i])
		{
			peg_results[peg_result_idx++] = 2;
			guess[i]=9999;
			white_peg_results[peg_color]--;
		}
	}
	// Now search for white pegs and remove them from the guess
	for(i=0;i<QTD_ANSWER;i++)
	{
		peg_color = guess[i];
		if(peg_color < QTD_PEGS)
		{
			if(white_peg_results[peg_color]>0)
			{
				peg_results[peg_result_idx++] = 1;
				white_peg_results[peg_color]--;
				guess[i] = 9999;
			}
		}
	}
	memcpy(board->results[board->guess], peg_results, sizeof(unsigned int)*QTD_ANSWER);
	board->guess++;
	return game_success;
}

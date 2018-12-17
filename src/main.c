#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "ga.h"

int main(int argc, char *argv[])
{
	int i,j;
	game_board game;

	GAME_CHECK(initialize_board(&game));
	unsigned int guess[QTD_ANSWER];
	unsigned int peg_results[QTD_ANSWER];
	unsigned int hits = 0;
	for(i=0;i<MAX_MOVES;i++)
	{
		hits = 0;
		fprintf(stdout,"%d >", i);
		fscanf(stdin, "%d %d %d %d", &guess[0], &guess[1], &guess[2], &guess[3]);
		GAME_CHECK(add_guess(&game, guess, peg_results));
		fprintf(stdout,"   ");
		for(j=0;j<QTD_ANSWER;j++)
		{
			fprintf(stdout,"%d ", peg_results[j]);
			if(peg_results[j] == 2)
				hits++;
		}
		fprintf(stdout,"\n");
		if(hits == QTD_ANSWER)
		{
			fprintf(stdout,"Congratulations!\n");
			break;
		}
	}
	fprintf(stdout,"All guesses:\n");
	for(i=0;i<MAX_MOVES;i++)
	{
		for(j=0;j<QTD_ANSWER;j++)
			fprintf(stdout,"%d ",game.guesses[i][j]);
		fprintf(stdout,"(");
		for(j=0;j<QTD_ANSWER;j++)
			fprintf(stdout,"%d ",game.results[i][j]);
		fprintf(stdout,")\n");
	}
	fprintf(stdout, "Secret: ");
	for(i=0;i<QTD_ANSWER;i++)
		fprintf(stdout,"%d ", game.secret[i]);
	fprintf(stdout,"\n");

	return EXIT_SUCCESS;
}

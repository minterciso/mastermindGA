#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "game.h"
#include "ga.h"

int main(int argc, char *argv[])
{
	int i,j,k;
	game_board game;
	individual *pop = NULL;

	srand(time(NULL));
	pop = create_population();
	GAME_CHECK(initialize_board(&game));
	unsigned int hits = 0;
	unsigned int peg_results[QTD_ANSWER];
	unsigned int *guess = NULL;
	fprintf(stdout,"Secret: ");
	for(k=0;k<QTD_ANSWER;k++)
		fprintf(stdout,"%d",game.secret[k]);
	fprintf(stdout,"\n");
	for(i=0;i<POP_SIZE;i++)
	{
		fprintf(stdout,"Individual %04d\n",i);
		for(j=0;j<MAX_MOVES;j++)
		{
			hits = 0;
			guess = pop[i].strategy[j];
			for(k=0;k<QTD_ANSWER;k++)
				fprintf(stdout,"%d", guess[k]);

			GAME_CHECK(add_guess(&game,guess,peg_results));
			fprintf(stdout,"(");
			for(k=0;k<QTD_ANSWER;k++)
			{
				fprintf(stdout,"%d", peg_results[k]);
				if(peg_results[k] == 2)
					hits++;
			}
			fprintf(stdout,")");
			if(hits==QTD_ANSWER)
			{
				fprintf(stdout,"Found in %d moves. [",j);
				for(k=0;k<QTD_ANSWER;k++)
					fprintf(stdout,"%d ",game.secret[k]);
				fprintf(stdout,"]\n");
				break;
			}
			fprintf(stdout,"\n");
		}
		fprintf(stdout,"\n");
		GAME_CHECK(reset_guesses(&game));
	}
	if(pop != NULL)
		free(pop);
	return EXIT_SUCCESS;
}

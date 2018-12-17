#include <stdio.h>
#include <stdlib.h>

#include "ga.h"

int main(int argc, char *argv[])
{
	individual *pop = NULL;
	pop = create_population();
	int i,j,k;
	for(i=0;i<POP_SIZE;i++)
	{
		fprintf(stdout, "%d ", i);
		for(j=0;j<MAX_MOVES;j++)
		{
			fprintf(stdout,"[");
			for(k=0;k<QTD_ANSWER;k++)
				fprintf(stdout,"%d",pop[i].strategy[j][k]);
			fprintf(stdout,"]");
		}
		fprintf(stdout, "(%0.2f)\n", pop[i].fitness);
	}
	free(pop);
	return EXIT_SUCCESS;
}

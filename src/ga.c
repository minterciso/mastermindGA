#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "consts.h"
#include "ga.h"

individual* create_population(void)
{
	int i,j,k;
	individual *pop = NULL;
	size_t pop_bytes = sizeof(individual)*POP_SIZE;

	if((pop=(individual*)malloc(pop_bytes))==NULL)
	{
		perror("malloc");
		return NULL;
	}
	memset(pop, '\0', pop_bytes);
	srand(time(NULL));
	for(i=0;i<POP_SIZE;i++)
		for(j=0;j<MAX_MOVES;j++)
			for(k=0;k<QTD_ANSWER;k++)
				pop[i].strategy[j][k] = rand() % QTD_PEGS;
	return pop;
}
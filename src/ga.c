#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "consts.h"
#include "ga.h"
#include "ca.h"

individual* create_population(void)
{
    individual *pop = NULL;
    size_t popBytes = sizeof(individual)*POP_SIZE;
    int k=QTD_PEGS, r=2;

    if((pop=(individual*)malloc(popBytes))==NULL)
    {
        perror("malloc");
        return NULL;
    }
    memset(pop, '\0', popBytes);
    for(int i=0;i<POP_SIZE;i++)
    {
        if((pop[i].rule = create_random_rule(k,r,&pop[i].ruleSize))==NULL)
        {
            fprintf(stderr,"Unable to create random rule!\n");
            free(pop);
            return NULL;
        }
    }
    return pop;
}

void fitness(individual *pop)
{
}

int select_individual(individual *pop, selection_type type)
{
}

void crossover_and_mutate(individual *pop, selection_type type)
{
}

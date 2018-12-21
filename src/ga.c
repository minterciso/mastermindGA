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
    for(i=0;i<POP_SIZE;i++)
        for(j=0;j<MAX_MOVES;j++)
            for(k=0;k<QTD_ANSWER;k++)
                pop[i].strategy[j][k] = rand() % QTD_PEGS;
    return pop;
}

void fitness(individual *pop)
{
    int i,j,k;
    float fitness = 0.0;
    float sumFitness = 0.0;
    for(i=0;i<POP_SIZE;i++)
    {
        fitness = 0.0;
        for(j=0;j<MAX_MOVES;j++)
            for(k=0;k<QTD_ANSWER;k++)
                fitness += (float)pop[i].results[j][k];
        fitness /= pop[i].moves;
        pop[i].fitness = fitness;
        sumFitness += fitness;
    }
    // Now normalize
    for(i=0;i<POP_SIZE;i++)
        pop[i].fitness = pop[i].fitness/sumFitness;
}

int select_individual(individual *pop, selection_type type)
{
    int idx = 0;
    if(type == elite_only)
        idx = rand()%KEEP_POP;
    return idx;
}

void crossover_and_mutate(individual *pop, selection_type type)
{
    int i,j,k;
    int p1_idx, p2_idx;
    int move1_idx, move2_idx;
    individual *p1, *p2, *s1, *s2;
    float rnd = 0.0;

    if(type == elite_only)
    {
        // We should remove everything that's not elite material
        for(i=KEEP_POP; i<POP_SIZE;i++)
            memset(&pop[i], '\0', sizeof(individual));
    }
    // Now create new individuals
    for(i=KEEP_POP;i<POP_SIZE; i+=2)
    {
        p1_idx = select_individual(pop, type);
        p2_idx = select_individual(pop, type);
        p1 = &pop[p1_idx];
        p2 = &pop[p2_idx];
        s1 = &pop[i];
        s2 = &pop[i+1];
        // copy the moves from the parents, and swap 2 distinct moves beetween the parents, for each movement
        //memcpy(s1->strategy, p1->strategy, sizeof(unsigned int)*MAX_MOVES*QTD_ANSWER);
        //memcpy(s2->strategy, p2->strategy, sizeof(unsigned int)*MAX_MOVES*QTD_ANSWER);
        memcpy(s1, p1, sizeof(individual));
        memcpy(s2, p2, sizeof(individual));
        s1->fitness = 0.0;
        for(j=0;j<MAX_MOVES;j++){
            move1_idx = rand() % QTD_ANSWER;
            move2_idx = rand() % QTD_ANSWER;
            s1->strategy[j][move1_idx] = p2->strategy[j][move1_idx];
            s2->strategy[j][move2_idx] = p1->strategy[j][move2_idx];
        }
        // Now we mutate
        for(j=0;j<MAX_MOVES;j++)
        {
            for(k=0;k<QTD_ANSWER;k++)
            {
                rnd = (float)rand()/(float)(RAND_MAX);
                if(rnd < PROB_MUTATION)
                    s1->strategy[j][k] = rand() % QTD_ANSWER;
                rnd = (float)rand()/(float)(RAND_MAX);
                if(rnd < PROB_MUTATION)
                    s2->strategy[j][k] = rand() % QTD_ANSWER;
            }
        }
    }
}

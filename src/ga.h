#ifndef __GA_H
#define __GA_H

#include "consts.h"

typedef struct
{
    float fitness;
    int moves;
    unsigned int strategy[MAX_MOVES][QTD_ANSWER];
    unsigned int results[MAX_MOVES][QTD_ANSWER];
} individual;

typedef enum selection_type
{
    tournament,
    elite_only
} selection_type;

individual* create_population(void);
void fitness(individual *pop);
void crossover_and_mutate(individual *pop, selection_type type);
int select_individual(individual *pop, selection_type type);

#endif //__GA_H

#ifndef __GA_H
#define __GA_H

#include "consts.h"

typedef struct
{
    float fitness;
    int moves;
    char *rule;
    double ruleSize;
} individual;

typedef enum selection_type
{
    tournament,
    elite_only
} selection_type;

individual* create_population(void);
void destroy_population(individual *pop);
void fitness(individual *pop);
void crossover_and_mutate(individual *pop, selection_type type);
int select_individual(individual *pop, selection_type type);

#endif //__GA_H

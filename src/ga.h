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

individual* create_population(void);
void fitness(individual *pop);

#endif //__GA_H

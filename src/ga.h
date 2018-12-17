#ifndef __GA_H
#define __GA_H

#include "consts.h"

typedef struct
{
    float fitness;
    unsigned int strategy[MAX_MOVES][QTD_ANSWER];
} individual;

individual* create_population(void);

#endif //__GA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

#include "game.h"
#include "ga.h"

static int cmpind(const void *p1, const void *p2)
{
    individual *s1 = (individual*)p1;
    individual *s2 = (individual*)p2;
    if(s1->fitness < s2->fitness)
        return 1;
    return 0;
}

void debug(const char *fmt, ...)
{
#ifdef DEBUG
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout,fmt, args);
    va_end(args);
#endif
}

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
#ifdef DEBUG
    debug("Secret: ");
    for(k=0;k<QTD_ANSWER;k++)
        debug("%d",game.secret[k]);
    debug("\n");
#endif
    for(i=0;i<POP_SIZE;i++)
    {
        debug("Individual %04d\n",i);
        for(j=0;j<MAX_MOVES;j++)
        {
            hits = 0;
            guess = pop[i].strategy[j];
#ifdef DEBUG
            for(k=0;k<QTD_ANSWER;k++)
                fprintf(stdout,"%d", guess[k]);
#endif
            GAME_CHECK(add_guess(&game,guess,peg_results));
            memcpy(pop[i].results[j], &peg_results, sizeof(unsigned int)*QTD_ANSWER);
            debug("(");
            for(k=0;k<QTD_ANSWER;k++)
            {
                debug("%d", peg_results[k]);
                if(peg_results[k] == 2)
                    hits++;
            }
            debug(")");
            if(hits==QTD_ANSWER)
            {
                pop[i].moves = j;
#ifdef DEBUG
                fprintf(stdout,"Found in %d moves. [",j);
                for(k=0;k<QTD_ANSWER;k++)
                    fprintf(stdout,"%d ",game.secret[k]);
                fprintf(stdout,"]\n");
#endif
                break;
            }
            pop[i].moves = j;
            debug("\n");
        }
        debug("\n");
        GAME_CHECK(reset_guesses(&game));
    }
    fitness(pop);
    qsort(pop,POP_SIZE, sizeof(individual), cmpind);
    for(i=0;i<POP_SIZE;i++)
        fprintf(stdout,"Individual %d: %f\n",i, pop[i].fitness);
    if(pop != NULL)
        free(pop);
    return EXIT_SUCCESS;
}

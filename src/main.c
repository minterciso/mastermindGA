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
    int i,j,k,g;
    game_board game;
    individual *pop = NULL;
    individual *best = NULL;

    srand(time(NULL));
    pop = create_population();
    GAME_CHECK(initialize_board(&game));
    unsigned int hits = 0;
    unsigned int peg_results[QTD_ANSWER];
    unsigned int *guess = NULL;
    unsigned int tmp_strategy[QTD_ANSWER];
#ifdef DEBUG
    debug("Secret: ");
    for(k=0;k<QTD_ANSWER;k++)
        debug("%d",game.secret[k]);
    debug("\n");
#endif
    for(g=0;g<MAX_GENERATIONS;g++)
    {
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
                // Save the guess before executing it
                memcpy(tmp_strategy, guess, sizeof(unsigned int)*QTD_ANSWER);
                GAME_CHECK(add_guess(&game,guess,peg_results));
                // Restore the guess (the correct pegs are changed to 9999
                memcpy(guess, tmp_strategy, sizeof(unsigned int)*QTD_ANSWER);
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
        best = &pop[0];
        fprintf(stdout, "Generation %03d: %0.10f\n", g, best->fitness);
        crossover_and_mutate(pop, elite_only);
    }
    // Best strategy
    fprintf(stdout,"Secret: ");
    for(i=0;i<QTD_ANSWER;i++)
        fprintf(stdout,"%d",game.secret[i]);
    fprintf(stdout,"\n");
    fprintf(stdout,"Best Fitness: %0.10f\n", best->fitness);
    fprintf(stdout,"Strategy:\n");
    for(i=0;i<MAX_MOVES;i++)
    {
        fprintf(stdout,"Move %d:", i);
        for(j=0;j<QTD_ANSWER;j++)
            fprintf(stdout,"%d", best->strategy[i][j]);
        fprintf(stdout,"\n");
    }
#ifdef DEBUG
    for(i=0;i<POP_SIZE;i++)
        fprintf(stdout,"Individual %d: %f\n",i, pop[i].fitness);
#endif
    if(pop != NULL)
        free(pop);
    return EXIT_SUCCESS;
}

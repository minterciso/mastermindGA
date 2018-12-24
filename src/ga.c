#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "consts.h"
#include "ga.h"
#include "ca.h"
#include "game.h"

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
    // The fitness function will create 100 games and execute the CA for each individual on THAT specific rule for MAX_MOVES steps.
    // The value of the fitness is the amount of correct guesses results, inverselly multiplied by the amount of moves it took to
    // guess. For instance, if it took 4 guesses to find the result, the fitness of that specific board game will be 1/4, or 0.25

    // Create the games
    game_board games[100];
    for(int j=0;j<100;j++)
        GAME_CHECK(initialize_board(&games[j]));

    int r=2;
    int k=QTD_PEGS;

    for(int i=0;i<POP_SIZE;i++)
    {
        char *rule = pop[i].rule;
        for(int j=0;j<100;j++)
        {
            char *lattice = NULL;
            // Create initial lattice
            lattice = create_initial_lattice(QTD_ANSWER, k);
            pop[i].moves = 0;
            games[j].guess=0;
            for(int m=0;m<MAX_MOVES;m++)
            {
                pop[i].moves++;
                // Execute one step of the CA
                lattice = execute_ca(lattice, QTD_ANSWER, rule, pop[i].ruleSize, r, k, 0);
                // Translate back from lattice to guess
                unsigned int guess[QTD_ANSWER];
                for(int k=0;k<QTD_ANSWER;k++) guess[k] = (int)lattice[k];
                // See if we have a valid result
                unsigned int results[QTD_ANSWER];
                GAME_CHECK(add_guess(&games[j], guess, results));
                int hits = 0;
                for(int k=0;k<QTD_ANSWER;k++)
                {
                    if(results[k]==2) hits++;
                }
                if(hits==QTD_ANSWER)
                    break;
            }
            pop[i].fitness += 1/pop[i].moves;
            free(lattice);
        }
        pop[i].fitness /= 100;
    }
}

int select_individual(individual *pop, selection_type type)
{
}

void crossover_and_mutate(individual *pop, selection_type type)
{
}

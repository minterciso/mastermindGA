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
    int k=QTD_PEGS, r=1;

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

void destroy_population(individual *pop)
{
    for(int i=0;i<POP_SIZE;i++)
    {
        if(pop[i].rule != NULL)
            free(pop[i].rule);
    }
    free(pop);
}

void fitness(individual *pop)
{
    // The fitness function will create 100 games and execute the CA for each individual on THAT specific rule for MAX_MOVES steps.
    // The value of the fitness is the amount of correct guesses results, inverselly multiplied by the amount of moves it took to
    // guess. For instance, if it took 4 guesses to find the result, the fitness of that specific board game will be 1/4, or 0.25

    // Create the games
    int amount_of_games = 100;
    game_board games[amount_of_games];

    int r=1;
    int k=QTD_PEGS;
    int circular = 1;

    for(int i=0;i<POP_SIZE;i++)
    {
        pop[i].fitness = 0.0;
        memset(&games, '\0', sizeof(game_board)*amount_of_games);
        for(int j=0;j<amount_of_games;j++)
            GAME_CHECK(initialize_board(&games[j]));
        char *rule = pop[i].rule;
        for(int j=0;j<amount_of_games;j++)
        {
            char *lattice = NULL;
            // Create initial lattice
            lattice = create_initial_lattice(QTD_ANSWER, k);
            pop[i].moves   = 0;
            games[j].guess = 0;
            for(int m=0;m<MAX_MOVES;m++)
            {
                pop[i].moves++;
                // Check how good the lattice is related to the secret code
                unsigned int guess[QTD_ANSWER];
                for(int k=0;k<QTD_ANSWER;k++) guess[k] = (int)lattice[k];
                // See if we have a valid result
                unsigned int results[QTD_ANSWER];
                GAME_CHECK(add_guess(&games[j], guess, results));
                int hits = 0;
                for(int k=0;k<QTD_ANSWER;k++)
                {
                    pop[i].fitness += (float)results[k];
                    //pop[i].fitness += ((float)pop[i].fitness * (float)results[k]);
                    if(results[k]==2){
                        //pop[i].fitness += 2.0;
                        hits++;
                    }
                }
                //pop[i].fitness *= (float)hits;
                if(hits==QTD_ANSWER)
                    break;
                // Execute one step of the CA
                lattice = execute_ca(lattice, QTD_ANSWER, rule, pop[i].ruleSize, r, k, circular);
            }
            if(pop[i].moves>0)
                pop[i].fitness *= (1.0/(float)pop[i].moves);
            free(lattice);
        }
        //pop[i].fitness /= amount_of_games;
    }
}

int select_individual(individual *pop, selection_type type)
{
    int idx = 0;
    if(type == elite_only)
        idx = rand() % KEEP_POP;
    return idx;
}

void crossover_and_mutate(individual *pop, selection_type type)
{
    if(type == elite_only)
    {
        // Keep only KEEP_POP individuals, and reset the others
        for(int i=KEEP_POP; i<POP_SIZE; i++)
        {
            //free(pop[i].rule);
            //memset(&pop[i].rule, '0', pop[i].ruleSize*sizeof(char));
            pop[i].fitness=0.0;
            pop[i].moves=0;
        }
    }
    // Now create new individuals
    for(int i=KEEP_POP; i<POP_SIZE; i+=2)
    {
        int p1_idx,p2_idx;
        individual *p1, *p2, *s1, *s2;
        p1_idx = select_individual(pop, type);
        p2_idx = select_individual(pop, type);
        p1 = &pop[p1_idx];
        p2 = &pop[p2_idx];
        s1 = &pop[i];
        s2 = &pop[i+1];
        int ruleSize = p1->ruleSize;
        // Allocate rule size on sons
        // Select random point for crossover
        int xp = rand() % ruleSize;
        for(int j=0;j<xp;j++)
        {
            s1->rule[j] = p1->rule[j];
            s2->rule[j] = p2->rule[j];
        }
        for(int j=xp;j<ruleSize;j++)
        {
            s1->rule[j] = p2->rule[j];
            s2->rule[j] = p1->rule[j];
        }
        // Now mutate
        float rnd = 0.0;
        for(int j=0;j<ruleSize;j++)
        {
            rnd = (float)rand()/(float)(RAND_MAX);
            if(rnd < PROB_MUTATION)
                s1->rule[j] = rand() % QTD_PEGS;
            rnd = (float)rand()/(float)(RAND_MAX);
            if(rnd < PROB_MUTATION)
                s2->rule[j] = rand() % QTD_PEGS;
        }
    }
}

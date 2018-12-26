#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

#include "game.h"
#include "ga.h"
#include "ca.h"

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

void print_population(individual *pop)
{
#ifdef DEBUG
    for(int i=0;i<POP_SIZE;i++)
    {
        fprintf(stdout, "[**] Individual %d:", i);
        for(int j=0;j<pop[i].ruleSize;j++)
            fprintf(stdout, "%d", pop[i].rule[j]);
        fprintf(stdout,"(%.10f)\n", pop[i].fitness);
    }
#endif
}

int main(int argc, char *argv[])
{
    individual *pop = NULL;
    individual *best = NULL;

    // Start the PRNG
    fprintf(stdout,"[*] Starting PRNG...");
    fflush(stdout);
    start_prng();
    fprintf(stdout,"[OK]\n");
    fflush(stdout);

    // Create random population
    fprintf(stdout,"[*] Creating random population...");
    fflush(stdout);
    if((pop=create_population())==NULL)
    {
        fprintf(stderr, "Unable to create population\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout,"[OK]\n");
    fflush(stdout);

    print_population(pop);
    FILE *fp = NULL;
    if((fp = fopen("output.csv", "w"))==NULL)
    {
        perror("fopen");
        destroy_population(pop);
        return EXIT_FAILURE;
    }
    fprintf(fp,"generation, fitness, rule\n");
    fflush(fp);

    for(int g=0;g<MAX_GENERATIONS;g++)
    {
        fprintf(stdout,"[*] Generation %03d ",g);
        fprintf(stdout,".");
        fitness(pop);

        fprintf(stdout,".");
        float tot_fitness = 0.0;
        for(int i=0;i<POP_SIZE;i++)
            tot_fitness += pop[i].fitness;
        for(int i=0;i<POP_SIZE;i++)
            pop[i].fitness /= tot_fitness;

        fprintf(stdout,".");
        qsort(pop, POP_SIZE, sizeof(individual), cmpind);
        fflush(stdout);

        best = &pop[0];
        fprintf(stdout,": %.10f", best->fitness);
        fprintf(fp, "%d,%.10f,",g,best->fitness);
	for(int i=0;i<best->ruleSize;i++) fprintf(fp, "%d", best->rule[i]);
	fprintf(fp, "\n");
	fflush(fp);

        crossover_and_mutate(pop, tournament);
        fprintf(stdout," [OK]\n");
        fflush(stdout);
    }
    fprintf(stdout,"[*] Best Rule: ");
    for(int i=0;i<best->ruleSize;i++)
        fprintf(stdout,"%d", best->rule[i]);
    fprintf(stdout,"\n");

    fprintf(stdout,"[*] Sample game\n");
    fflush(stdout);
    game_board game;
    unsigned int *lattice = NULL;
    unsigned int guess[QTD_ANSWER];
    unsigned int results[QTD_ANSWER];

    GAME_CHECK(initialize_board(&game));
    fprintf(stdout,"[*] Secret: ");
    for(int i=0;i<QTD_ANSWER;i++) fprintf(stdout,"%d", game.secret[i]);
    fprintf(stdout,"\n");
    fflush(stdout);
    lattice = create_initial_lattice(QTD_ANSWER, QTD_PEGS);
    for(int i=0;i<MAX_MOVES;i++)
    {
        memset(guess, 0, sizeof(unsigned int)*QTD_ANSWER);
        memset(results, 0, sizeof(unsigned int)*QTD_ANSWER);
        memcpy(&guess, lattice, sizeof(unsigned int)*QTD_ANSWER);
        //for(int k=0;k<QTD_ANSWER;k++) guess[k] = lattice[k];
        fprintf(stdout,"[*] Guess: ");
        for(int k=0;k<QTD_ANSWER;k++) fprintf(stdout,"%d", guess[k]);

        GAME_CHECK(add_guess(&game, guess, results));
        int hits = 0;
        for(int k=0;k<QTD_ANSWER;k++)
        {
            if(results[k]==2){
                hits++;
            }
        }
        fprintf(stdout, "(%d)\n", hits);
        if(hits==QTD_ANSWER)
        {
            fprintf(stdout,"[*] Got the result in %d moves!\n", i);
            break;
        }
        lattice = execute_ca(lattice, QTD_ANSWER, best->rule, best->ruleSize, 1, QTD_PEGS, 1);
    }
    free(lattice);
    fclose(fp);

    fprintf(stdout,"[*] Freeing population memory\n");
    destroy_population(pop);
    return EXIT_SUCCESS;
}

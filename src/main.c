#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

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

    fprintf(stdout,"[*] Calcullating fitness...");
    fflush(stdout);
    fitness(pop);
    fprintf(stdout,"[OK]\n");
    fflush(stdout);

    fprintf(stdout,"[*] Sorting...");
    fflush(stdout);
    qsort(pop, POP_SIZE, sizeof(individual), cmpind);
    fprintf(stdout,"[OK]\n");

    for(int i=0;i<POP_SIZE; i++)
        fprintf(stdout,"%d: %0.10f\n", i,pop[i].fitness);

    free(pop);
    return EXIT_SUCCESS;
}

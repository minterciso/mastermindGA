#include "ca.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "utils.h"

void start_prng(void)
{
    srand(time(NULL));
}

unsigned int* create_random_rule(unsigned int k, unsigned int r, double *ruleSize)
{
    // Each rule has a space of k^(2*r), and a search space of k^(k^(2*r))
    // So for instance a rule with k=2 and r=1 (2 colors and 1 radius) would result in a string of 2^3 positions, 8 random positions of 1 and 0
    // For k=6, r=2 for instance, that would be 6^5, ie 7776 positions
    unsigned int* rule = NULL;
    *ruleSize = pow(k,(2*r)+1);
    size_t ruleSize_bytes = sizeof(unsigned int)*(*ruleSize);

    if((rule=(unsigned int*)malloc(ruleSize_bytes))==NULL)
    {
        perror("malloc");
        return NULL;
    }
    memset(rule, 0, ruleSize_bytes);
    for(int i=0;i<*ruleSize;i++)
        rule[i] = rand() % k;
    return rule;
}

unsigned int *create_initial_lattice(unsigned int size, unsigned int k)
{
    unsigned int *lattice = NULL;
    size_t latSize = sizeof(unsigned int)*size;

    if((lattice=(unsigned int*)malloc(latSize))==NULL)
    {
        perror("malloc");
        return NULL;
    }
    memset(lattice, 0, latSize);
    for(int i=0;i<size;i++)
        lattice[i] = rand() % k;
    return lattice;
}

unsigned int *execute_ca(unsigned int *lattice, size_t lat_size, unsigned int *rule, double rule_size, int r, unsigned int k, int circular)
{
    // We should get r values, translate it to the decimal index of the rule, and set the output on the new lattice, then return the new lattice
    int idx;
    unsigned int result_idx;
    unsigned int *new_lattice = NULL;
    unsigned int *tmp = NULL;
    size_t tmpSize = r*2+1;
    size_t tmpBytes = sizeof(unsigned int)*tmpSize;
    size_t latBytes = sizeof(unsigned int)*lat_size;

    if((new_lattice=(unsigned int*)malloc(latBytes))==NULL)
    {
        perror("malloc");
        return NULL;
    }
    if((tmp=(unsigned int*)malloc(tmpBytes))==NULL)
    {
        perror("malloc");
        free(new_lattice);
        return NULL;
    }
    memset(new_lattice, 0, latBytes);
    memset(tmp, 0, tmpBytes);

    for(int i=0;i<lat_size;i++)
    {
        idx = i-r;
        for(int j=0;j<tmpSize;j++)
        {
            if(circular==1)
            {
                if(idx<0)
                    idx = lat_size+idx;
                if(idx>lat_size-1)
                    idx=0;
            }
            else
            {
                if(idx<0)
                    idx=0;
                if(idx>lat_size-1)
                    idx=lat_size-1;
            }
            tmp[j] = lattice[idx++];
            if(tmp[j] > 6)
                printf("What?\n");
        }
        result_idx = to_decimal(tmp, k, tmpSize);
        int id = rule_size - result_idx-1;
        new_lattice[i] = rule[id];
    }
    memcpy(lattice, new_lattice, latBytes);
    free(new_lattice);
    free(tmp);
    return lattice;
}

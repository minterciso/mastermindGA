#ifndef CA_H
#define CA_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Start the PRNG subsystem
 *
 * Currently this is using the standar C random function, but on the future we'll use GSL if available.
 */
void start_prng(void);

/**
 * @brief Create a random CA rule
 *
 * Based on the amount of colors (k) and radius (r), create a random rule and return a pointer to the rule representation. It'll also store the rule size on the ruleSize variable.
 * For instance, if k=2 and r=1 one valid return for this function can be: '0110011' and ruleSize will be 8.
 *
 * @param k The amount of colors
 * @param r The radius amount
 * @param ruleSize A pointer to a double variable that'll store the rule size
 * @return An array with the rule result as a char
 */
char *create_random_rule(unsigned int k, unsigned int r, double *ruleSize);

/**
 * @brief Create a random initial lattice of size size and colors k
 * @param size The size of the lattice
 * @param k The amount of colors of the lattice
 * @return A pointer to an array with the lattices
 */
char *create_initial_lattice(unsigned int size, unsigned int k);

/**
 * @brief Execute the CA for one step
 *
 * Based on the rule parameter, r radius and k color, execute the CA for one step on the lattice, and return a new lattice.
 *
 * @param lattice A pointer to an already initialized lattice of lat_size
 * @param lat_size The size of the lattice
 * @param rule The rule to execute, as returned from create_random_rule() function
 * @param rule_size The rule size
 * @param r The radius of the CA
 * @param k The colors of the CA
 * @param circular If we should treat the lattice as a circular lattice
 * @return The lattice with the CA already executed
 */
char *execute_ca(char *lattice, size_t lat_size, char *rule, double rule_size, int r, unsigned int k, int circular);

#endif // CA_H

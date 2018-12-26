#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int to_decimal(unsigned int *str, int base, int len)
{
    int power = 1; // Initialize power of base
    int num = 0;  // Initialize result
    int i;

    // Decimal equivalent is str[len-1]*1 +
    // str[len-1]*base + str[len-1]*(base^2) + ...
    for (i = len - 1; i >= 0; i--)
    {
        // A digit in input number must be
        // less than number's base
        if (str[i] >= base)
        {
            printf("Invalid Number");
            return -1;
        }

        num += str[i] * power;
        power = power * base;
    }

    return num;
}

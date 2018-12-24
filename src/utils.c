#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int to_decimal(char *str, int base, int len)
{
    int power = 1; // Initialize power of base
    int num = 0;  // Initialize result
    int i;
    char ch;

    // Decimal equivalent is str[len-1]*1 +
    // str[len-1]*base + str[len-1]*(base^2) + ...
    for (i = len - 1; i >= 0; i--)
    {
        // A digit in input number must be
        // less than number's base
        ch = str[i];
        int num_=(int)ch;
        if (num_ >= base)
        {
            printf("Invalid Number");
            return -1;
        }

        num += num_ * power;
        power = power * base;
    }

    return num;
}

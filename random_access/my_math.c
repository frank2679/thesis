#include <stdio.h>
#include <math.h>
#include "my_math.h"

/* combination computation */
double combination_compute(int n, int k){
    long long int a = 1, b = 1;
    int i = 0;
    /* compute n(n-1)(n-2)...(n-(n-k)) */
    for(; i < k; i++)
        a *= n-i;
    /* compute k! */
    for( i = 0; i < k; i++)
        b *= k-i;
//    printf ("(%d,%d), a, b: %lld, %lld, %lld \n", n,k,a/b, a, b);
    return a/b;
}

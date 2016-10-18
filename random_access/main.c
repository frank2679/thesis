/* This is mathematical analysis of 802.11ax random access procedure. 
 *
 *
 * Bug: when n is not integer, it is the same with the value of integral part. 
 *
 *
 *
 * Frank 
 * 2016
 */
#include <stdio.h>
#include <math.h>
#include "myhead.h"

#define PRECISE 0.000001
int OCW = 18; // initial contention window size. 
int m = 3; // backoff levels
int N = 15; // # of STAs need contend
int n = 10; // # of STAs sending BSR at current stage
int M = 9; // # of RUs for random access


int main(){
    /* as index for "for" loop */
    int i = 0;
    /* kmax is the up bound of k; */
    int kmax = 0;
    if (M < n)
        kmax = M;
    else 
        kmax = n;

    double p = 0.0; 
    for (; i < kmax+1; i++){
        p += pmf_suc_contend(i);
    }
    printf("total prob. = %.3f\n", p);

    /* calculate tau and p */
    /*
    double tau = 0.0;
    p = solve_tau_p(n, PRECISE); // collision probability
    tau = (1-pow(1-p, 1.0/(n-1.0)))*M; 
    printf("collsion probability: %.3f\n", p);
    printf("transmission probability: %.3f\n", tau);
    */
    //* to put into file *//
    FILE *fp_1;
    int Num_sta[9] = {2,5,10,20,30,50,70,90,100}; // for estimate dense scenario problem

    double tau[9] = {0};

    fp_1 = fopen("n_tau_p.dat", "a");
    fprintf(fp_1, "# backoff stages: %d \n", m);
    for (i = 0; i < 9; i++){
    /* tau vs n */
        p = solve_tau_p(Num_sta[i], PRECISE);
        tau[i] = (1-pow(1-p, 1.0/(Num_sta[i]-1.0)))*M;
        fprintf(fp_1, "%d     %.5f    %.5f\n", Num_sta[i], tau[i], p);
    }


    fclose(fp_1);
    return 0;
}

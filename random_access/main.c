/* This is mathematical analysis of 802.11ax random access procedure. 
 *
 *
 * Bug: when n is not integer, it is the same with the value of integral part. 
 * Experience: 
 *      1. different comment level using multiple slashes 
 *      2. global variable name should be short, while local variable name should be long.
 *
 *
 * Frank 
 * 2016
 */
#include <stdio.h>
#include <math.h>
#include "myhead.h"

#define PRECISE 0.000001
double OCW = 18;           // initial contention window size. 
double m = 3;              // backoff levels
double n = 0;             // # of STAs sending BSR at current stage
double M = 9;              // # of RUs for random access
double tau = 0.0;          // prob. of transmission of STA
double p = 0.0;          // prob. of transmission of STA



int main(){
    /* total parameters */
    int     i = 0;          // as index for "for" loop
    int     Num_sta_input[9] = {2,5,10,20,30,50,70,90,100}; // for estimate dense scenario problem
    double  Ps = 0.0;       // prob. of success contend in a stage
    double  ns = 0.0;       // # of STAs suc contend in a stage
    double  stages = 0.0;   // # of stages until a suc stage

    // test [ 
    /*
    n = 5.0;
    for (i = 0;i < n+1; i++){
        p += pmf_suc_contend(i);
    }
    printf("total prob.: %.3f\n", p);
    return 0;
    */
    // end test ] 


    FILE *fp_1, *fp_2, *fp_3, *fp_4;

    fp_1 = fopen("n_tau_p.dat", "a");
    fp_2 = fopen("n_Ps.dat", "a");
    fp_3 = fopen("n_ns.dat", "a");
    fp_4 = fopen("n_stages.dat", "a");

    fprintf(fp_1, "# backoff stages: %.3f \n", m);
    fprintf(fp_1, "# initial OCW: %.3f \n", OCW);

    fprintf(fp_2, "# initial OCW: %.3f \n", OCW);
    fprintf(fp_2, "# initial OCW: %.3f \n", OCW);

    fprintf(fp_3, "# backoff stages: %.3f \n", m);
    fprintf(fp_3, "# initial OCW: %.3f \n", OCW);

    fprintf(fp_4, "# backoff stages: %.3f \n", m);
    fprintf(fp_4, "# initial OCW: %.3f \n", OCW);


    for (n = 1; n < 101; n++){

        /* calculate tau and p */
        p = solve_tau_p(PRECISE);
        if (n > 1)
            tau = (1-pow(1-p, 1.0/(n-1.0)))*M;
        else
            tau = (OCW+1.0)/(3.0*OCW-M+1.0);
        fprintf(fp_1, "%.3f     %.5f    %.5f\n", n, tau, p);

        /* calculate Ps, ns, # of stages until a suc stage */ 
        Ps = compute_Ps();
        fprintf(fp_2, "%.3f       %.5f\n", n, Ps);

        ns = compute_ns();
        fprintf(fp_3, "%.3f       %.5f\n", n, ns);

        stages = compute_stages_suc(Ps);
        fprintf(fp_4, "%.3f       %.5f\n", n, stages);
    }
    fclose(fp_1);
    

    return 0;
}

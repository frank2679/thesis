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

    fp_1 = fopen("n_tau_p_math.dat", "a");
    fp_2 = fopen("n_Nstation_math.dat", "a"); // # of stages for a STA to succeed 
    fp_3 = fopen("n_ns_math.dat", "a");       // # of suc stations in a stage
    fp_4 = fopen("n_Nstage_math.dat", "a"); // # of stages until a success stage

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
        if (n > 1){
            p = solve_tau_p(PRECISE);
            tau = compute_tau_1(p); // simpler but not fit for "n=1" 
        }
        else{
            p = 0;
            tau = compute_tau_2(p);
        }
        fprintf(fp_1, "%.3f     %.5f    %.5f\n", n, tau, p);

        /* calculate Ps, ns, # of stages until a suc stage */ 
        Ps = compute_Ps(); // prob. of a station succeeding contending 
        fprintf(fp_2, "%.3f       %.5f\n", n, 1.0/Ps);

        ns = compute_ns();
        fprintf(fp_3, "%.3f       %.5f\n", n, ns);

        stages = compute_stages_suc();
        fprintf(fp_4, "%.3f       %.5f\n", n, stages);
    }
    fclose(fp_1);
    

    return 0;
}

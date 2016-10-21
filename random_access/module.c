#include <stdio.h>
#include <math.h>
#include "myhead.h"

extern double OCW; // initial window size
extern double m; // # of contention window levels
extern double n; // # of STAs sending BSR at current stage
extern double M; // # of RUs for random access
extern double tau;
extern double p;

/* for problem of r balls in n cells */
double pmf_suc_contend(double k){ // k cells which only contain single ball
    /* compute the prob. */
    double p = 0.0; // just the prob.
    double j = k;
    double a1 = 0.0; // coefficient
    double a2 = 0.0;
    for (; j < M; j++){
        a1 = combination_compute((int) j,(int) k);
        a2 = S_j(j);
        p += pow(-1,k+j)*a1*a2; 
    }
    // printf("prob. of exact %d STAs contend success: %.3f\n", (int)k, p);
    return p;
}

//* functions for pmf_suc_contend() *//
double S_j(double j){
    double i = 0.0;
    long int a1 = combination_compute((int)M, (int)j);
    double s = a1;
    for (;i < j; i++){
        s *= (ceil(n*tau)-i)/(M-i)*pow(1.0-1.0/(M-i), ceil(n*tau)-i-1); // n*tau is the actually contending STAs in a stage
    }
    return s;
}

/* for saturated analysis of 802.11ax random access */
//* numerical method to find solution of equation groups *// // subsection-comment
double solve_tau_p(double Precise){
    double p = 0, tau = 0;
    double first = 0.0, last = 1.0, mid = 0.5; // dichotomy method  
    double f1 = 0.0, f2 = 0.0, f3 = 0.0; 

    //* for special case n = 1 *//
    if (n == 1){ // n=1, p = 0;
        //printf("special case\n");
        return 0;
    }

    for (;;){
        mid = (first + last)/2.0;
        f1 = equation (first);
        f2 = equation (mid);
        f3 = equation (last);
        //
        if (fabs(f1) <= Precise){
            p = first;
            break;
        }
        else 
            if (fabs(f2) <= Precise){
                p = mid;
                break;
            }
            else 
                if (fabs(f3) <= Precise){
                    p = last;
                    break;
                }
        //
        if (f1*f2 > 0){
            first = mid;
        }
        else 
            if (f2*f3 > 0){
                last = mid;
            }
    }
    //printf("p:  %.6f; \ntau:    %.6f;\n", p, tau);
    return p;
}

//* group equations of tau and p *// // f = f1 - f2
double equation(double p){
    double result = 0.0; 
    result = (1-pow(1.0-p,1.0/(n-1.0)))*M;
    double sum = 0.0;
    int i = 1;
    for (; i < m+1; i++){
        sum += pow(p/2.0, i);
    }
    result -= (OCW+1)/ (3*OCW -M +1 +(2+M)*sum);

    return result;
}

/* compute Ps & ns */
/* calculate Ps & ns */ 
/* Ps: prob. of success contend in a stage
 * ns: # of STAs success contend in a stage
 */
//* compute Ps *//
double compute_Ps(){
    double prob_no_suc_stage = 0.0;
    double prob_suc_stage = 0.0;

    prob_no_suc_stage = pmf_suc_contend(0);
    //printf("n= %.3f,  Ps(0): %.3f\n", n, prob_no_suc_stage);
    prob_suc_stage = 1.0 - prob_no_suc_stage;
    return prob_suc_stage;
}

//* compute ns *//
double compute_ns(){
    /* kmax is the up bound of k; */
    double kmax = 0.0; // the most # of suc STAs in a stage
    double num_suc_STA = 0.0;
    if (M < n)
        kmax = M;
    else 
        kmax = n;

    int i = 1;
    for (; i < kmax+1; i++){
        num_suc_STA += i*pmf_suc_contend(i);
    }
    /* second way to compute mean of ns */
    //num_suc_STA = ceil(tau*n)*pow(1.0-1.0/M, ceil(tau*n)-1);
    
    /* third way to compute mean of ns */
    num_suc_STA = (1-p)*tau*n;
    //printf("expected # of success STAs in a stage = %.3f\n", num_suc_STA);
    return num_suc_STA;
}

//* compute # of stages until suc stage *//
double compute_stages_suc(double prob_suc_stage){ 
    return 1.0/prob_suc_stage;
}

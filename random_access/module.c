#include <stdio.h>
#include <math.h>
#include "myhead.h"

extern int OCW; // initial window size
extern int m; // # of contention window levels
extern int N; // # of STAs need contend
extern int n; // # of STAs sending BSR at current stage
extern int M; // # of RUs for random access

/* for problem of r balls in n cells */
double pmf_suc_contend(int k){
    /* compute the prob. */
    double p = 0.0; // just the prob.
    int j = k;
    int a1 = 0; // coefficient
    double a2 = 0.0;
    for (; j < M; j++){
        a1 = combination_compute(j,k);
        a2 = S_j(j);
        p += pow(-1,k+j)*a1*a2; 
    }
    printf("prob. of exact %d STAs contend success: %.3f\n", k, p);
    return p;
}

double S_j(int j){
    double i = 0.0;
    long int a1 = combination_compute(M,j);
    double s = a1;
    for (;i < j; i++){
        s *= (n-i)/(M-i)*pow(1-1.0/(M-i), n-i-1);
    }
    return s;
}

/* for saturated analysis of 802.11ax random access */
//* numerical method to find solution of equation groups *// // subsection-comment
double solve_tau_p(int n, double Precise){
    double p = 0, tau = 0;
    double first = 0.0, last = 1.0, mid = 0.5; // dichotomy method  
    double f1 = 0.0, f2 = 0.0, f3 = 0.0; 

    for (;;){
        mid = (first + last)/2.0;
        f1 = equation (first, n);
        f2 = equation (mid, n);
        f3 = equation (last, n);
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
double equation(double p, int n){
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

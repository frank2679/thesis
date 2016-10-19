#include "my_math.h"
/* problem of r balls in n cells */
double S_j(double j); // prob. of at least j STAs who success contend
double pmf_suc_contend(double k); // prob. of exact k STAs who success contend

/* for saturated analysis of random access */
double solve_tau_p(double Precise);
double equation(double p);

/* calculate Ps & ns & level */ 
/* Ps: prob. of success contend in a stage
 * ns: # of STAs success contend in a stage
 * stages_suc: # stages until a suc stage 
 */
double compute_Ps();
double compute_ns();
double compute_stages_suc(double prob_suc_stage); 

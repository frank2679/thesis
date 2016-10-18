#include "my_math.h"
/* problem of r balls in n cells */
double S_j(int j); // prob. of at least j STAs who success contend
double pmf_suc_contend(int k); // prob. of exact k STAs who success contend
/* for saturated analysis of random access */
double solve_tau_p(int, double);
double equation(double p, int num_sta);
double solve_throughput(double tau, int num_sta);
double solve_energy_efficiency(int num_sta);

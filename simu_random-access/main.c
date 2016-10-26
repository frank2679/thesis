/* This is simulation of 802.11ax random access procedure. 
 *
 *
 *
 *
 * Frank 
 * 2016
 */
#include <stdio.h>
#include <math.h>
#include "myhead.h"

/* system parameters */
int n = 0; // number of stations
int M = 0; // number of RUs in a stage
int m = 0; // backoff levels
int OCW = 0; // initial window

/* system state */
long int curr_stage = 0;
long int end_stage = 0;
int back_level[100] = {0}; // only the first element is -1. 
int OBO[100] = {0}; 
int selected_RU[100] = {0}; 
int active_STA[100] = {0}; 
int suc_STA[100] = {0}; // store index of STAs who succeed
int col_STA[100] = {0}; // store index of STAs who collide 

/* statistics */
long int num_stage = 0;
long int num_fail_stage = 0;
long int num_suc_stage = 0;
long int num_suc_STA = 0;
long int num_attempt = 0; // number of attempt in total, to compute tau
long int num_suc_attempt = 0;// number of suc attempt of a given STA, to compute p
int r_num_suc_STA = 0; // real time value, # of suc STA in single stage
int r_num_active_STA = 0; // real time value, # of active STA in single stage
double tau = 0.0;
double p = 0.0;

int main(){
    FILE *fp_1, *fp_2, *fp_3, *fp_4;
    fp_1 = fopen("n_ns_simu.dat", "a");
    fp_2 = fopen("n_Nstage_simu.dat", "a");
    fp_3 = fopen("n_Nstation_simu.dat", "a");
    fp_4 = fopen("n_tau_p_simu.dat", "a");
    fprintf(fp_1, "#n      ns\n");
    fprintf(fp_2, "#n      Ns_stage\n");
    fprintf(fp_3, "#n      Ns_station\n");
    fprintf(fp_4, "#n      tau       p\n");
    srand(time(NULL));
    n = 1;
    m = 3;
    M = 9;
    OCW = 18;
    for (n = 1; n < 101; n++){
        simulate(n, M, m, OCW, 500000); // n, M, m, OCW, end
        data_analysis(fp_1, fp_2, fp_3, fp_4);
    }
    fclose(fp_1);
    fclose(fp_2);
    fclose(fp_3);
    fclose(fp_4);

    return 0;
}

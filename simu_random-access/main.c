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
int num_stage = 0;
int num_fail_stage = 0;
int num_suc_stage = 0;
int num_suc_STA = 0;

int main(){
    FILE *fp;
    fp = fopen("n_ns_stages.dat", "a");
    fprintf(fp, "n      ns      Ns_stage\n");
    srand(time(NULL));
    n = 100;
   // for (n = 5; n < 101; n++){
        simulate(n, 9, 3, 18, 4000000); // n, M, m, OCW, end
        data_analysis(fp);
  //  }
    fclose(fp);

    return 0;
}

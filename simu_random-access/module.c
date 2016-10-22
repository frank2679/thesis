#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "myhead.h"

/* system parameters */
extern int n; // number of stations
extern int M; // number of RUs in a stage
extern int m; // backoff levels
extern int OCW; // initial window

/* system state */
extern long int curr_stage;
extern long int end_stage;
extern int back_level[100]; 
extern int OBO[100]; 
extern int selected_RU[100]; 
extern int active_STA[100]; 
extern int suc_STA[100];
extern int col_STA[100];

/* statistics */
extern int num_stage;
extern int num_fail_stage;
extern int num_suc_stage;
extern int num_suc_STA;

/* system level */
void system_initial(int n_l, int M_l, int m_l, int OCW_l, int end_l){ // initialize system parameters 
    curr_stage = 1;
    end_stage = end_l;
    n = n_l;
    M = M_l;
    m = m_l;
    OCW = OCW_l;
    // reset all the system state
    int i = 0;
    for (; i < 101; i++){
        back_level[i] = -1;
        OBO[i] = -1;
        selected_RU[i] = -1;
        active_STA[i] = -1;
        suc_STA[i] = -1; 
        col_STA[i] = -1; 
    }
    for (i = 0; i < n; i++){
        back_level[i] = 0;
        OBO[i] = 0;
        selected_RU[i] = 0; // value is among [1,M]
        active_STA[i] = 1; // 0 means non-active, 1 is active, -1 not work
        suc_STA[i] = 0;  // assume all station are suc
        col_STA[i] = 0; 
    }
    generate_OBO();
}

void simulate(int n_l, int M_l, int m_l, int OCW_l, int end_l){ // this funtion just can run a total simulation with given parameters
    system_initial(n_l, M_l, m_l, OCW_l, end_l);
//    printf("system initialized \n");
//    display_system_state();
    while (curr_stage <= end_stage){
        decrease_OBO();

        update_active_STA();

        generate_selected_RU();
 //       printf("generate selected RU\n");
  //      display_system_state();

        set_suc_STA();
  //      printf("set_suc_STA\n");
  //      display_system_state();

        set_back_level();
  //      printf("set backoff level\n");
  //      display_system_state();

        record_statistic();

        update_system_state();
  //      printf("update system state\n");
  //      display_system_state();

  //      printf("num_suc_STA: %3d, num_suc_stage: %3d \n\n\n", num_suc_STA, num_suc_stage);
    }

}

void display_system_state(){
    int i = 0;
    printf("---------------------------\n");
    printf("curr_stage: %ld,  n: %d,  m: %d,  M:  %d,\n", curr_stage, n, m, M);
    /* backoff level */
    printf("backoff level   :");
    for (i = 0; i < n+1; i++){
        printf("%3d  ", back_level[i]);
    }
    printf("\n");

    /* OBO */
    printf("OBO             :");
    for (i = 0; i < n+1; i++){
        printf("%3d  ", OBO[i]);
    }
    printf("\n");

    /* active STA*/
    printf("active STA      :");
    for (i = 0; i < n+1; i++){
        printf("%3d  ", active_STA[i]);
    }
    printf("\n");
    
    /* seletecd RU */
    printf("selected RU     :");
    for (i = 0; i < n+1; i++){
        printf("%3d  ", selected_RU[i]);
    }
    printf("\n");

    /* suc STA */
    printf("suc STA         :");
    for (i = 0; i < n+1; i++){
        printf("%3d  ", suc_STA[i]);
    }
    printf("\n");
    /* col STA */
    printf("col STA         :");
    for (i = 0; i < n+1; i++){
        printf("%3d  ", col_STA[i]);
    }
    printf("\n---------------------------\n\n");
}

/* data analysis */
void data_analysis(FILE *fp){
    double ns = 0.0; 
    double Ns_stage = 0.0; 
    num_stage = num_fail_stage + num_suc_stage;
    if (num_stage == end_stage)
        printf("time validated.\n");
    else
        printf("time not validated.\n");

    ns = (double) (num_suc_STA)/ (double) num_stage;// -n is because the initialization add n num_suc_STA
    Ns_stage = (double) num_stage/ (double) num_suc_stage;
    fprintf(fp, "%d     %.3f     %.3f\n", n, ns, Ns_stage);
    printf("num_suc_stage: %3d,  ns: %.3f,     Ns_stage: %.3f\n", num_suc_stage, ns, Ns_stage);
}

/* operation */
void generate_OBO(){
    int i = 0;
    int curr_OCW;
    for (; i < n; i++){
        if (active_STA[i] == 1){
            curr_OCW = pow(2, back_level[i])*(OCW+1)-1;
            OBO[i] = random_generator(curr_OCW);
            active_STA[i] = 0; // reset the active_STA[]
        }
    }
    //display_system_state();
}

void decrease_OBO(){
    int i = 0;
//    printf("before decrease\n");
//    display_system_state();
    for (; i < n; i++){
        OBO[i] -= M;
        if (OBO[i] < 0)
            OBO[i] = 0;
    }
//    printf("after decrease\n");
//    display_system_state();
}

void update_active_STA(){
    int i = 0;
    for (; i < n; i++){
        if(!OBO[i]){
            active_STA[i] = 1;
        }
    }
//    display_system_state();
}

void generate_selected_RU(){
    int i = 0;
    for (; i < n; i++){
        if (active_STA[i] > 0)
            selected_RU[i] = random_generator(M-1)+1;
    }
//    display_system_state();
}

void set_suc_STA(){
    int i = 0;
    int j = 0;
    bool col_STA_flag = false; // for collision of a station
    bool suc_stage_flag = false; // for suc stage
    for (; i < n; i++){
        col_STA_flag = false; // for collision of a station
        if (active_STA[i] > 0 && col_STA[i] == 0){// not yet same with previous STAs
            for (j = i+1; j < n; j++){
                if (active_STA[j] > 0 ){ 
                    if (same(selected_RU[i], selected_RU[j])){
                        col_STA[i] = 1;
                        col_STA[j] = 1;
                        col_STA_flag = true;
                    }
                }
            }
            if (!col_STA_flag){
                suc_STA[i] = 1;
                suc_stage_flag = true; // at least one station suc means suc of stage
                col_STA_flag = false;
            }
        }
    }
    //* statistic *//
    if (suc_stage_flag){
        num_suc_stage++;
        suc_stage_flag = false;
    }
    else
        num_fail_stage++;

}

void record_statistic(){
    int i = 0;
    for (; i < n; i++){
        if (suc_STA[i] > 0){
            num_suc_STA++;
        }
    }
}

void set_back_level(){
    int i = 0;
    for (; i < n; i++){
        if (suc_STA[i] > 0){
            back_level[i] = 0;
        }
        else if (col_STA[i] > 0 && back_level[i] < m){
            back_level[i] += 1;
        }
    }
}

void update_system_state(){
    /* update clock */
    curr_stage++;
    int i = 0;
    for (; i < n; i++){
        /* update active_STA */
        suc_STA[i] = 0;

        /* update col_STA */
        col_STA[i] = 0; // reset the col_STA[]

        /* update selected RU */
        selected_RU[i] = 0;
    }
    /* generate next OBO */
    generate_OBO();
}

/* tool functions */
int random_generator(int max){ // random: [0,max]
    return rand()%(max+1);
}

bool same(int a, int b){
    if (a == b)
        return true;
    else
        return false;
}

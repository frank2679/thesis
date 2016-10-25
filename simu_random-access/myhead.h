// #include <stdio.h>
#include <stdbool.h>
/* system level */
void simulate(int n_l, int M_l, int m_l, int OCW_l, int end_l);
void system_initial(int, int, int, int, int); // initialize system parameters 
void display_system_state();
/* data analysis */
void data_analysis(FILE *, FILE *, FILE *);

/* operation */
void generate_OBO();
void decrease_OBO();
void update_active_STA();
void generate_selected_RU();
void set_suc_STA();
void record_statistic();
void set_back_level();
void reset_suc_STA();
void update_system_state();
/* tool function */
int random_generator(int max);
bool same(int a, int b);

#include <stdio.h>
#include <math.h>
#define Pre         0.00001
#define Ini_win     32
#define Back_stg    6

double solve_tau_p(int, double);
double equation(double p, int n);

int main(){
    FILE *fp;
    fp = fopen("n_tau_p.dat", "a");
    int Num_sta[9] = {2,5,10,20,30,50,70,90,100}; // for estimate dense scenario problem
    double tau,p;
    int i = 0;
    for (; i < 9; i++){
        p = solve_tau_p(Num_sta[i], Pre);
        tau = 1-pow(1-p, 1.0/(Num_sta[i]-1.0));
        fprintf(fp, "%d     %.3f    %.3f\n", Num_sta[i], tau, p);
    }
    fclose(fp);

    return 0; 
}

/* functions */
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
// equations for solving tau
double equation(double p, int n){
    double result = 0.0; 
    result = 1-pow(1-p,1.0/(n-1.0))-(1-Ini_win);
    double sum = 0.0;
    int i = 0;
    for (; i < Back_stg; i++){
        sum += pow(2.0*p, i);
    }
    result -= p*Ini_win*sum;

    return result;
}

#include <stdio.h>
#include <math.h>
#define Pre         0.0000001
#define P           8184
#define T_s         8982
#define T_c         8713 // H+P+DIFS+theta
#define T_t         8584 // tramsmission time of one transmission, H+P
#define ACK         240  // time to receive a ACK.
#define DIFS        128
#define SIFS        28
#define theta       50
#define Wt          1200 // power of transmit /mA
#define Wr          800
#define Wi          500

int Ini_win = 32;
int Back_stg = 3;

double p_tr = 0.0;
double p_s = 0.0;
double avg_slot = 0;
double St = 0.0;    // sta's partition in transmit state
double Sr = 0.0;
double Si = 0.0;

double solve_tau_p(int, double);
double equation(double p, int num_sta);
double solve_throughput(double tau, int num_sta);
double solve_energy_efficiency(int num_sta);

int main(){
    FILE *fp_1, *fp_2, *fp_3;
    int Num_sta[9] = {2,5,10,20,30,50,70,90,100}; // for estimate dense scenario problem

    double tau[9] = {0}, p;
    double S[9] = {0};
    double energy_eff = 0.0;

    fp_1 = fopen("n_tau_p.dat", "a");
    fp_2 = fopen("n_throughput.dat", "a");
    fp_3 = fopen("n_energy_eff.dat", "a");
    fprintf(fp_1, "# backoff stages: %d \n", Back_stg);
    fprintf(fp_2, "# backoff stages: %d \n", Back_stg);
    fprintf(fp_3, "# backoff stages: %d \n", Back_stg);
    fprintf(fp_3, "# n   St       Sr        Si        Et        Er          Ei        Eff\n");
    int i = 0;
    for (; i < 9; i++){
    /* tau vs n */
        p = solve_tau_p(Num_sta[i], Pre);
        tau[i] = 1-pow(1-p, 1.0/(Num_sta[i]-1.0));
        fprintf(fp_1, "%d     %.5f    %.5f\n", Num_sta[i], tau[i], p);

    /* throughput vs n */ // for validation of Bianchi
        S[i] = solve_throughput(tau[i], Num_sta[i]);
        fprintf(fp_2, "%d     %.5f\n", Num_sta[i], S[i]);

    /* energy efficiency estimate: J/bit */
        energy_eff = solve_energy_efficiency(Num_sta[i]);
        fprintf(fp_3, "%d     %.4f    %.4f  %.4f    %.4f    %.4f    %.4f    %.4f\n", 
                Num_sta[i], St, Sr, Si, St*Wt, Sr*Wr, Si*Wi, energy_eff);
    }
    fclose(fp_1);
    fclose(fp_2);
    fclose(fp_3);

    return 0; 
}
/* energy efficiency estimate */
double solve_energy_efficiency(n){
    St = p_tr / n * T_t / avg_slot;
    Si = ((1-p_tr)*theta + p_tr*p_s*(DIFS+SIFS)+p_tr*(1-p_s)*SIFS)/avg_slot; 
    Sr = 1 - St - Si;
    double energy = 0.0;
    energy = St*Wt + Sr*Wr + Si*Wi;
    double sentBit = 0.0;
    sentBit = p_s*p_tr*P/n;
    printf("avg slot time: %.3f\n", avg_slot);
    printf("St: %.3f    Sr: %.3f    Si: %.3f\n", St, Sr, Si);
    printf("Ptr: %.3f   Ps: %.3f\n", p_tr, p_s);
    printf("(1-Ptr)theta: %.3f  PtrPsTs: %.3f   Ptr(1-Ps)Tc: %.3f\n", (1-p_tr)*theta, p_tr*p_s*T_s, p_tr*(1-p_s)*T_c);
    printf("bits sent in a slot time: %.3f\n\n", sentBit);
    return energy/sentBit;
}


/* throughput */
double solve_throughput(double tau, int n){
    p_tr = 1-pow(1-tau, n);
    p_s = n * tau * pow(1-tau, n-1)/ p_tr;
    avg_slot = ((1-p_tr)*theta + p_tr*p_s*T_s + p_tr*(1-p_s)*T_c);
    double S = 0.0;
    S = p_tr * p_s * P/avg_slot; 
    return S;
}

/* functions{ */
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
    result = 1-pow(1-p,1.0/(n-1.0));
    double sum = 0.0;
    int i = 0;
    for (; i < Back_stg; i++){
        sum += pow(2.0*p, i);
    }
    result -= 2/ (1 + Ini_win + p*Ini_win*sum);

    return result;
}
//}

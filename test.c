#include <complex.h>
#include <raylib.h>
#include <stdio.h>

void fft(const float _Complex* x_entree, int n, int s, float _Complex* x_sortie){
    if(n == 1)
        x_sortie[0] = x_entree[0];
    else{
        fft(x_entree, n/2, 2*s, x_sortie);
        fft(x_entree+s, n/2, 2*s, x_sortie+(n/2));
        for(int i = 0; i < ((n/2)); i++){
            float _Complex p = x_sortie[i];
            float _Complex q = cexpf(((-2*PI*I)/n)*i)*x_sortie[i+(n/2)];
            x_sortie[i] = p + q;
            x_sortie[i+(n/2)] = p - q;
        }
    }
}

int main(void){
    float _Complex x[8] = {1,0,0,0,0,0,0,0};
    float _Complex y[8] = {0,0,0,0,0,0,0,0};

    fft(x, 8, 1, y);

    for(int i = 0; i < 8; i++){
        printf("%.3f + %.3fi\n", crealf(y[i]), cimagf(y[i]));
    }
}

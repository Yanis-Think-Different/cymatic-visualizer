#include "../include/audio.h"
#include <complex.h>
#include <raylib.h>

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

int peak(float _Complex* tab, int taille){
    float max = cabsf(tab[0]);
    int indice = 0;
    for(int i = 1; i < taille/2; i++){
        float valeur = cabsf(tab[i]);
        indice = (valeur > max) ? i : indice;
    }
    return indice;
}

int conversion_hz(int indice_max, int samp, int taille_trame){
    return (indice_max * samp) / taille_trame;
}

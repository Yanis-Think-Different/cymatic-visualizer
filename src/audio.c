#include "../include/audio.h"
#include <complex.h>
#include <math.h>
#include <raylib.h>

#define NB_COUPLES (N_MAX * (N_MAX - 1) / 2)
#define N_MAX 10

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

static int comparer_couples(const void* a, const void* b) {
    const Couple* ka = (const Couple*)a;
    const Couple* kb = (const Couple*)b;
    return ka->key - kb->key;
}

void harmonisation(Couple* val, float frequence){
    static Couple table[NB_COUPLES];
    static int initialise = 0;

    //cette double boucle cree plein de couple avec leurs clé respectif
    // jusque NB couple qui represente le nombre de couple audio que je possede
    // pour de la richesse visuel et apres on classe du plus petit au plus grand
    if (!initialise) {
        int idx = 0;
        for (int n = 1; n <= N_MAX; n++) {
            for (int m = 1; m < n; m++) {
                table[idx].m = m;
                table[idx].n = n;
                table[idx].key = m * m + n * n;
                idx++;
            }
        }
        qsort(table, NB_COUPLES, sizeof(Couple), comparer_couples);
        initialise = 1;
    }

    float frequence_min = 50;
    float frequence_max = 4000;
    if (frequence < frequence_min)
        frequence = frequence_min;
    else if (frequence > frequence_max)
        frequence = frequence_max;

    float t = (logf(frequence/frequence_min)/logf(frequence_max/frequence_min));

    //On selectionne le couple le plus approprie
    int index = (int)roundf(t * ((int)NB_COUPLES - 1));
    if (index < 0) index = 0;
    if (index >= NB_COUPLES) index = NB_COUPLES - 1;

    *val = table[index];
}

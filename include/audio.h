#ifndef AUDIO
#define AUDIO

#include <stdlib.h>
#include <complex.h>
#include "outils.h"

void fft(const float _Complex* x_entree, int n, int s, float _Complex* x_sortie);

int peak(float _Complex* tab, int taille);

int conversion_hz(int indice_max, int samp, int taille_trame);

void harmonisation(Couple* val, float frequence);

#endif

#ifndef AUDIO
#define AUDIO

#include <stdlib.h>
#include <complex.h>

void fft(const float _Complex* x_entree, int n, int s, float _Complex* x_sortie);

#endif

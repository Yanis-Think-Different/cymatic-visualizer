#ifndef PHYSIC
#define PHYSIC

#include <stdlib.h>
#include "outils.h"

typedef struct grain{
    Vector pos;
    Vector dir;
}Grain;

float amplitude(float x, float y, int m, int n, int lenght, int height);

void new_direction(Grain* other, float ampli_u, float ampli_d, float ampli_l, float ampli_r);

void maj_pos(Grain* other, float amp, float delta_T);
    
#endif
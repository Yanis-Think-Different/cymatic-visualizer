#include "../include/physic.h"

float amplitude(float x, float y, ModeActif modes[4], int lenght, int height){
    float amp_totale = 0.0f;
    for(int i = 0; i < 4; i++){
        if (modes[i].poids < 0.01f) continue;
        int m = modes[i].val.m;
        int n = modes[i].val.n;
        float chladni = cosf((n*PI*x)/lenght) * cosf((m*PI*y)/height) + cosf((m*PI*x)/lenght) * cosf((n*PI*y)/height);
        amp_totale += modes[i].poids * chladni;
    }
    return amp_totale;
}

void new_direction(Grain* other, float ampli_u, float ampli_d, float ampli_l, float ampli_r){
    Vector dir;

    float x = difference(ampli_l, ampli_r);
    float y = difference(ampli_u, ampli_d);

    dir.x = x;
    dir.y = y;

    other->dir = dir;
}

void maj_pos(Grain* other, float amp, float delta_T){
    Vector anc_pos = other->pos;
    Vector dir_amelio = mul_vect_val(mul_vect_val(normalisation(other->dir), amp), delta_T);
    Vector new_pos = sub_vector(anc_pos, dir_amelio);

    float a = 3;
    float bruit_x = ((float)rand() / (float)RAND_MAX) * 2.0f * a - a;
    float bruit_y = ((float)rand() / (float)RAND_MAX) * 2.0f * a - a;

    new_pos.x += bruit_x;
    new_pos.y += bruit_y;

    other->pos = new_pos;
}

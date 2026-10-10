#include "../include/physic.h"

float amplitude(float x, float y, int m, int n, int lenght, int height){
    return cos((n*PI*x)/lenght) * cos((m*PI*y)/height) - cos((m*PI*x)/lenght) * cos((n*PI*y)/height);
}

void new_direction(Grain* other, float ampli_u, float ampli_d, float ampli_l, float ampli_r){
    Vector dir;

    float x = difference(ampli_d, ampli_l);
    float y = difference(ampli_u, ampli_d);

    dir.x = x;
    dir.y = y;

    other->pos = dir;
}

void maj_pos(Grain* other, float amp, float delta_T){
    Vector anc_pos = other->pos;
    if (amp < 0)
        amp = amp * -1;
    Vector dir_amelio = mul_vect_val(mul_vect_val(other->dir, amp), delta_T);
    Vector new_pos = sub_vector(anc_pos, dir_amelio);
    other->pos = new_pos;
}

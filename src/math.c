#include "math.h"
#include "../include/outils.h"

float difference(float ampli_1, float ampli_2){
    return ampli_1 - ampli_2;
}

Vector sub_vector(Vector un, Vector deux){
    Vector sub;
    sub.x = un.x - deux.x;
    sub.y = un.y - deux.y;
    return sub;
}

Vector mul_vect_val(Vector un, float val){
    Vector mul;
    mul.x = un.x * val;
    mul.y = un.y * val;
    return mul;
}

Vector normalisation(Vector other){
    Vector norm;
    float longeur = sqrtf(pow(other.x,2) + pow(other.y,2));
    if(!longeur){
        norm.x = 0;
        norm.y = 0;
        return norm;
    }
    norm.x = other.x / longeur;
    norm.y = other.y / longeur;
    return norm;
}

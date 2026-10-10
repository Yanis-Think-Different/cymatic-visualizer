#include "physic.h"

float amplitude(float x, float y, int m, int n, int lenght, int height){
    return cos((n*PI*x)/lenght) * cos((m*PI*y)/height) - cos((m*PI*x)/lenght) * cos((n*PI*y)/height);
}
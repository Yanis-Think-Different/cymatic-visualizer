#ifndef OUTIL
#define OUTIL

#define PI 3.14

typedef struct vector{
    float x;
    float y;
}Vector;

typedef struct Constante{
    float pas;
    float delta_T;
    int n;
    int m;
}Const;

typedef struct Param{
    int nombre_grain;
    int lenght;
    int height;
}Parametre;

#endif

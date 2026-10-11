#ifndef OUTIL
#define OUTIL

typedef struct vector{
    float x;
    float y;
}Vector;

typedef struct Constante{
    float pas;
    float delta_T;
}Const;

typedef struct Param{
    int nombre_grain;
    int lenght;
    int height;
}Parametre;

typedef struct Couple{
    int m;
    int n;
    int key;
}Couple;

#endif

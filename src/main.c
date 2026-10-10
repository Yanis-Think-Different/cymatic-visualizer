#include "../include/physic.h"
#include "../include/outils.h"
//#include "../include/audio.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>

#define HEIGHT 720
#define LENGHT 1280
#define GRAIN_NB 10000

void argument_recuperation(int nb_args, char* tab_args[], Parametre* param);

void calcul(Parametre* param,Const* constante ,Grain *grain);

void init_tab_grain(Parametre* param, Grain grains[]);

//////////////////////////////////////

int main(int argc, char* argv[]){
    srand(1234);

    Parametre param;
    Const constante;
    char* fenetre = "cynamic-visualizer";

    param.height = HEIGHT;
    param.lenght = LENGHT;
    param.nombre_grain = GRAIN_NB;

    constante.delta_T = 1;
    constante.pas = 1;
    constante.m = 40;
    constante.n = 30;

    if(argc > 1)
        argument_recuperation(argc, argv, &param);

    Grain grains[param.nombre_grain];
    init_tab_grain(&param, grains);

    InitWindow(param.lenght, param.height, fenetre);
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        for(int i = 0; i < param.nombre_grain; i++)
            calcul(&param, &constante, &grains[i]);
        BeginDrawing();
            ClearBackground(BLACK);
            for(int i = 0; i < param.nombre_grain; i++){
                Vector2 vect;
                vect.x = grains[i].pos.x;
                vect.y = grains[i].pos.y;
                DrawPixelV(vect, WHITE);
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

//////////////////////////////////////

void argument_recuperation(int nb_args, char* tab_args[], Parametre* param){
    for (int i = 1; i < nb_args; i++){
        if(tab_args[i][0] == '-'){
            switch(tab_args[i][1]) {
                case 'H':
                    i++;
                    param->height = atoi(tab_args[i]);
                    break;
                case 'L':
                    i++;
                    param->lenght = atoi(tab_args[i]);
                    break;
                case 'G':
                    i++;
                    param->nombre_grain = atoi(tab_args[i]);
                    break;
                default:
                    break;
            }
        }
        else {
            printf("erreur au niveau des arguments");
            exit(EXIT_FAILURE);
        }
    }
}

void calcul(Parametre* param,Const* constante ,Grain *grain){
    float amp_u = fabsf(amplitude(grain->pos.x, (grain->pos.y + constante->pas), constante->m, constante->n, param->lenght, param->height));
    float amp_d = fabsf(amplitude(grain->pos.x, (grain->pos.y - constante->pas), constante->m, constante->n, param->lenght, param->height));
    float amp_l = fabsf(amplitude((grain->pos.x + constante->pas), grain->pos.y, constante->m, constante->n, param->lenght, param->height));
    float amp_r = fabsf(amplitude((grain->pos.x - constante->pas), grain->pos.y, constante->m, constante->n, param->lenght, param->height));
    float moy_amp = (amp_d + amp_l + amp_r + amp_u) / 4;

    new_direction(grain, amp_u, amp_d, amp_l, amp_r);
    maj_pos(grain, moy_amp, constante->delta_T);
}

void init_tab_grain(Parametre* param, Grain grains[]){
    for(int i = 0; i < param->nombre_grain; i++){
        float random_x = ((float)rand() / (float)RAND_MAX) * param->lenght;
        float random_y = ((float)rand() / (float)RAND_MAX) * param->height;

        grains[i].pos.x = random_x;
        grains[i].pos.y = random_y;
    }
}

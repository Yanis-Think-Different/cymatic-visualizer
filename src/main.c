#include "raylib.h"
#include "../include/maths.h"
#include "../include/physic.h"
#include "../include/outils.h"
#include "../include/audio.h"
#include "../include/affichage.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 720
#define LENGHT 1280
#define GRAIN_NB 50000

void argument_recuperation(int nb_args, char* tab_args[], Parametre* param);

void calcul(Parametre* param,Const* constante ,Grain* grain);

//////////////////////////////////////

int main(int argc, char* argv[]){
    srand(time(NULL));

    Parametre param;
    Const constante;

    param.height = HEIGHT;
    param.lenght = LENGHT;
    param.nombre_grain = GRAIN_NB;

    constante.delta_T = 1;
    constante.pas = 1;
    constante.m = 56;
    constante.n = 165;

    if(argc > 1)
        argument_recuperation(argc, argv, &param);

    Grain* grains = malloc(param.nombre_grain * sizeof(Grain));


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

void calcul(Parametre* param,Const* constante ,Grain* grain){
    float amp_u = amplitude(grain->pos.x, (grain->pos.y + constante->pas), constante->m, constante->n, param->lenght, param->height);
    float amp_d = amplitude(grain->pos.x, (grain->pos.y - constante->pas), constante->m, constante->n, param->lenght, param->height);
    float amp_l = amplitude((grain->pos.x + constante->pas), grain->pos.y, constante->m, constante->n, param->lenght, param->height);
    float amp_r = amplitude((grain->pos.x - constante->pas), grain->pos.y, constante->m, constante->n, param->lenght, param->height);
    float moy_amp = (amp_d + amp_l + amp_r + amp_u) / 4;

    new_direction(grain, amp_u, amp_d, amp_l, amp_r);
    maj_pos(grain, moy_amp, constante->delta_T);
}

#include "raylib.h"
#include "../include/maths.h"
#include "../include/physic.h"
#include "../include/outils.h"
#include "../include/audio.h"
#include "../include/affichage.h"
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 720
#define LENGHT 1280
#define GRAIN_NB 50000

void argument_recuperation(int nb_args, char* tab_args[], Parametre* param);

int main(int argc, char* argv[]){
    Parametre param;
    param.height = HEIGHT;
    param.lenght = LENGHT;
    param.nombre_grain = GRAIN_NB;

    if(argc > 1)
        argument_recuperation(argc, argv, &param);

    printf("%d\n",param.height);
    printf("%d\n",param.lenght);
    printf("%d\n",param.nombre_grain);

    return 0;
}

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

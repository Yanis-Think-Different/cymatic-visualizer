#include "../include/physic.h"
#include "../include/outils.h"
#include "../include/audio.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <complex.h>

#define HEIGHT 720
#define LENGHT 1280
#define GRAIN_NB 10000
#define TRAME 2048

void argument_recuperation(int nb_args, char* tab_args[], Parametre* param, char* fichier);

void calcul(Parametre* param,Const* constante ,Grain* grain, Couple* val);

void init_tab_grain(Parametre* param, Grain grains[]);

//////////////////////////////////////

int main(int argc, char* argv[]){
    srand(1234);

    //// INIT param generaux
    Parametre param;
    Const constante;
    Couple valeur;
    char* fichier = "music/rubedo.mp3";
    char* fenetre = "cynamic-visualizer";
    param.height = HEIGHT;
    param.lenght = LENGHT;
    param.nombre_grain = GRAIN_NB;
    constante.delta_T = 10;
    constante.pas = 1;
    bool pause = false;

    if(argc > 1)
        argument_recuperation(argc, argv, &param, fichier);

    //// INIT param variable
    Grain grains[param.nombre_grain];
    init_tab_grain(&param, grains);

    ////// INIT screen
    InitWindow(param.lenght, param.height, fenetre);
    SetTargetFPS(60);

    ////// INIT music
    InitAudioDevice();
    Music music = LoadMusicStream(fichier);
    PlayMusicStream(music);
    Wave metadonne = LoadWave(fichier);
    WaveFormat(&metadonne, metadonne.sampleRate, 32, 1);
    float _Complex* tab = (float _Complex*)LoadWaveSamples(metadonne);
    int taille_tab = metadonne.frameCount;

    while (!WindowShouldClose()){

        ////// Music
        UpdateMusicStream(music);
        // Commande de pause et de restart
        if (IsKeyPressed(KEY_SPACE)){
            StopMusicStream(music);
            PlayMusicStream(music);
        }
        if (IsKeyPressed(KEY_P)){
            pause = !pause;

            if (pause) PauseMusicStream(music);
            else ResumeMusicStream(music);
        }
        //Operation d'Analyse de transo et d'harmonisation pour recup le n et le m
        // on recup l'indice de depart au moment present de la music
        int indice_begin = GetMusicTimePlayed(music) * metadonne.sampleRate;
        //Tant qu'on est pas à la fin de la music on fait les transfo
        if (indice_begin + TRAME < taille_tab){
            // On cree deux buffer pour stocke le son actuelle et appluie une tranformation pour obtenir un spectre
            float _Complex buffer_entry[TRAME];
            float _Complex buffer_exit[TRAME];
            for(int i = 0; i < TRAME; i++)
                buffer_entry[i] = tab[indice_begin+i];
            //le n correspond à la taille de mon buffer et on met s à 1 (on lit tout des le depart)
            fft(buffer_entry, TRAME, 1, buffer_exit);
            int max = peak(buffer_exit, TRAME);
            float frequence = conversion_hz(max, metadonne.sampleRate, TRAME);
            harmonisation(&valeur, frequence);
        }

        ////// Calcul
        for(int i = 0; i < param.nombre_grain; i++)
            calcul(&param, &constante, &grains[i], &valeur);

        ////// Dessin
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

    UnloadMusicStream(music);
    UnloadWaveSamples((float*)tab);
    UnloadWave(metadonne);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

//////////////////////////////////////

void argument_recuperation(int nb_args, char* tab_args[], Parametre* param, char* fichier){
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
                case 'M':
                    i++;
                    fichier = tab_args[i];
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

void calcul(Parametre* param,Const* constante ,Grain* grain, Couple* val){
    float amp_u = fabsf(amplitude(grain->pos.x, (grain->pos.y + constante->pas), val->m, val->n, param->lenght, param->height));
    float amp_d = fabsf(amplitude(grain->pos.x, (grain->pos.y - constante->pas), val->m, val->n, param->lenght, param->height));
    float amp_l = fabsf(amplitude((grain->pos.x + constante->pas), grain->pos.y, val->m, val->n, param->lenght, param->height));
    float amp_r = fabsf(amplitude((grain->pos.x - constante->pas), grain->pos.y, val->m, val->n, param->lenght, param->height));
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

#include "../include/physic.h"
#include "../include/outils.h"
#include "../include/audio.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>

#define HEIGHT 1000
#define LENGHT 1000
#define GRAIN_NB 25000
#define TRAME 2048

void argument_recuperation(int nb_args, char* tab_args[], Parametre* param, char* fichier);

void calcul(Parametre* param, Const* constante, Grain* grain, ModeActif modes[4]);

void init_tab_grain(Parametre* param, Grain grains[]);

//////////////////////////////////////

int main(int argc, char* argv[]){
    srand(time(NULL));

    //// INIT param generaux
    Parametre param;
    Const constante;
    ModeActif modes[4] = { 0 };
    char* fichier = "music/flat.mp3";
    char* fenetre = "cynamic-visualizer";
    param.height = HEIGHT;
    param.lenght = LENGHT;
    param.nombre_grain = GRAIN_NB;
    constante.delta_T = 0.1;
    constante.pas = 1;
    bool pause = false;
    float volume = 0.8f;

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
    SetMusicVolume(music, volume);
    Wave metadonne = LoadWave(fichier);
    WaveFormat(&metadonne, metadonne.sampleRate, 32, 1);
    float* tab = LoadWaveSamples(metadonne);
    int taille_tab = metadonne.frameCount;
    float frequence;

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
        if (IsKeyDown(KEY_DOWN)){
            volume -= 0.05f;
            if (volume < 0.0f) volume = 0.0f;
            SetMusicVolume(music, volume);
        }
        else if (IsKeyDown(KEY_UP)){
            volume += 0.05f;
            if (volume > 1.0f) volume = 1.0f;
            SetMusicVolume(music, volume);
        }

        //Operation d'Analyse de transo et d'harmonisation pour recup le n et le m
        // on recup l'indice de depart au moment present de la music
        int indice_begin = GetMusicTimePlayed(music) * metadonne.sampleRate;
        //Tant qu'on est pas à la fin de la music on fait les transfo
        if (indice_begin + TRAME < taille_tab){
            float _Complex buffer_entry[TRAME];
            float _Complex buffer_exit[TRAME];
            for (int i = 0; i < TRAME; i++)
                buffer_entry[i] = tab[indice_begin + i] + 0.0f * I;

            fft(buffer_entry, TRAME, 1, buffer_exit);

            // Découpage en 4 bandes : sub/basse [1..8], bas-médium [9..45], médium [46..180], aigu [181..500]
            int bornes[5] = { 1, 9, 46, 181, 501 };

            for(int b = 0; b < 4; b++){
                float max_mag = 0.0f;
                int idx_max = bornes[b];

                for(int i = bornes[b]; i < bornes[b+1]; i++){
                    float mag = cabsf(buffer_exit[i]);
                    if (mag > max_mag){
                        max_mag = mag;
                        idx_max = i;
                    }
                }

                frequence = conversion_hz(idx_max, metadonne.sampleRate, TRAME);
                harmonisation(&modes[b].val, frequence);
                modes[b].poids = max_mag;
            }
        }


        printf("%f, %d, %d\n", frequence, modes->val.m, modes->val.n);

        ////// Calcul
        for(int i = 0; i < param.nombre_grain; i++)
            calcul(&param, &constante, &grains[i], modes);

        ////// Dessin
        BeginDrawing();
            ClearBackground(BLACK);
            for(int i = 0; i < param.nombre_grain; i++){
                Vector2 vect;
                vect.x = grains[i].pos.x;
                vect.y = grains[i].pos.y;
                DrawPixelV(vect, WHITE);
            }
            DrawFPS(10,10);
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

void calcul(Parametre* param, Const* constante, Grain* grain, ModeActif modes[4]){
    float amp_u = fabsf(amplitude(grain->pos.x, (grain->pos.y + constante->pas), modes, param->lenght, param->height));
    float amp_d = fabsf(amplitude(grain->pos.x, (grain->pos.y - constante->pas), modes, param->lenght, param->height));
    float amp_l = fabsf(amplitude((grain->pos.x + constante->pas), grain->pos.y, modes, param->lenght, param->height));
    float amp_r = fabsf(amplitude((grain->pos.x - constante->pas), grain->pos.y, modes, param->lenght, param->height));
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

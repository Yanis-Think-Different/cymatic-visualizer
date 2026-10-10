#ifndef PHYSIC
#define PHYSIC

#include <stdlib.h>
#include "outils.h"

typedef struct grain{
    Vector pos;
    Vector dir;
}Grain;

/*
 * amplitude
 * Calcule l'amplitude de vibration de la plaque au point (x, y),
 * selon la formule de Chladni pour le mode (n, m).
 * x, y        : position sur la plaque, dans [0, lenght] x [0, height]
 * m, n        : entiers définissant le mode de vibration (m != n)
 * lenght      : largeur de la plaque
 * height      : hauteur de la plaque
 * Retour      : amplitude signée (peut être négative). 0 = ligne nodale.
 */
float amplitude(float x, float y, int m, int n, int lenght, int height);

/*
 * new_direction
 * Calcule la direction dans laquelle le grain doit se déplacer pour
 * aller vers une zone qui vibre moins, et l'écrit dans other->dir.
 * La position du grain n'est pas modifiée.
 * other       : grain dont on met à jour la direction
 * ampli_1     : |A| en (x + h, y)
 * ampli_2     : |A| en (x - h, y)
 * ampli_3     : |A| en (x, y + h)
 * ampli_4     : |A| en (x, y - h)
 * Les 4 amplitudes doivent être passées en valeur absolue.
 */
void new_direction(Grain* other, float ampli_u, float ampli_d, float ampli_l, float ampli_r);

/*
 * maj_pos
 * Déplace le grain d'un pas de simulation dans la direction other->dir.
 * Le déplacement est proportionnel à amp et à delta_T : plus la zone
 * vibre, plus le grain bouge.
 * other->dir doit avoir été mis à jour par new_direction avant l'appel.
 * other       : grain à déplacer
 * amp         : |A| à la position actuelle du grain
 * delta_T     : durée du pas de simulation
 */
void maj_pos(Grain* other, float amp, float delta_T);
    
#endif
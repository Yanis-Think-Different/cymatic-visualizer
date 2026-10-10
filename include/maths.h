#ifndef MATHS
#define MATHS

#include <stdlib.h>
#include "outils.h"

/*
 * difference
 * Renvoie ampli_1 - ampli_2.
 * Fonction générique, l'ordre des arguments détermine le signe.
 */
float difference(float ampli_1, float ampli_2);

/*
 * Soustarction entre deux vecteurs
 * La fonction renvoie un vecteur
 */
Vector sub_vector(Vector un, Vector deux);

/*
 * multiplication entre vecteur et une valeur
 * La fonction renvoie un vecteur
 */
Vector mul_vect_val(Vector un, float val);

#endif

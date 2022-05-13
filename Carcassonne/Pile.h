#ifndef __PILE_H__
#define __PILE_H__


#include "Tuile.h"

struct pile_t
{
  int nb_tuile;
  int nb_tuile_max;
  Tuile* T;
};

typedef struct pile_t Pile;

Pile* init_pile();
Pile* creer_pile ();
int empiler_tuile(Pile* P, Tuile T);
Tuile* depiler_tuile (Pile *P);
void afficher_pile (Pile* P);
void free_pile (Pile* P);

#endif
#ifndef __PLATEAU_H__
#define __PLATEAU_H__

Tuile** creer_plateau ();
int pose_tuile (Tuile** plateau, Tuile *T, int ligne, int colonne);


void free_plateau (Tuile** Plateau);
void affichage_plateau (Tuile ** plateau);

int update_plateau (Tuile** plateau, Tuile* T);

#endif
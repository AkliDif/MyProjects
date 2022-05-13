#ifndef __PLATEAU_H__
#define __PLATEAU_H__

#define color(param) printf("\033[%sm",param)

Tuile** creer_plateau ();
int pose_tuile (Tuile** plateau, Tuile *T, int ligne, int colonne, int num_player);
int pose_tuile_depart(Tuile** plateau, Tuile *T, int i, int j, int num_player);
void free_plateau (Tuile** Plateau);
void affichage_plateau (Tuile ** plateau);

int update_plateau (Tuile** plateau, Tuile* T);
int route (Tuile** P, Tuile* T, int ligne, int colonne);

#endif
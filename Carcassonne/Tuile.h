#ifndef __TUILE_H__
#define __TUILE_H__

#define NB_TUILE_MAX 72
#define NB_MAX_MEEPLE 7


struct cote_t
{
    char type;
    int num_meeple;
    int cotient_meeple;
};

typedef struct cote_t Cote;



struct tuile_t
{
    Cote* cote_A;
    Cote* cote_B;
    Cote* cote_C;
    Cote* cote_D;
    Cote* cote_E;
    int vide, available, id_tuile;
};

typedef struct tuile_t Tuile;

int pose_meeple (Tuile **P, int ligne, int colonne, int num_meeple, int cote);

Tuile* creer_tuile (char cote_A, char cote_B, char cote_C, char cote_D, char cote_E, int id_tuile);
void free_tile (Tuile *T);
Tuile* get_tiles_from_file ();
void melanger_tuile (Tuile* temp);
void print_tile (Tuile *T);
Tuile* rotate_tuile(Tuile* T);


#endif 
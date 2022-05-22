#ifndef __TUILE_H__
#define __TUILE_H__

#define NB_TUILE_MAX 72
#define NB_MAX_MEEPLE 7


struct meeple_t
{
    int num_player;
    int coleur;
};

typedef struct meeple_t Meeple;

struct cote_t
{
    char type[10];
    Meeple* Pion;
    int vide;
};

typedef struct cote_t Cote;



struct tuile_t
{
    Cote* cote_A;
    Cote* cote_B;
    Cote* cote_C;
    Cote* cote_D;
    Cote* cote_E;
    int Joue, vide, nb, available, id_tuile;
    int num_player;
    //struct tuile_t *geauche, *droite, *haute, *bas; pour l'affichage tuile par tuile (pas en tableau)

};

typedef struct tuile_t Tuile;


int pose_meeple (Tuile *T, Meeple *M, int cote, int num_player);
Meeple* create_meeples (int num_player);

Tuile* creer_tuile (char cote_A[10], char cote_B[10], char cote_C[10], char cote_D[10], char cote_E[10], int num, int id_tuile);
void free_tile (Tuile *T);
Tuile* get_tiles_from_file ();
void melanger_tuile (Tuile* temp);
void print_tile (Tuile *T);
Tuile* rotate_tuile(Tuile* T);


#endif 
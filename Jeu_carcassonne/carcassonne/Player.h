#ifndef __PLAYER_H__
#define __PLAYER_H__



typedef struct player_t
{
    char Pseudo[256];
    int Num, Score;
    int color; 
}Player;


Player* create_player (unsigned int num);
void print_player (Player Pl);
int add_tile_to_player (Player* Pl, Tuile* T);

#endif 
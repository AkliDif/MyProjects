#ifndef __PLAYER_H__
#define __PLAYER_H__



struct player_t
{
    char Pseudo[256];
    int Num, Score; 
};

typedef struct player_t Player;


Player* create_player (unsigned int num);
void print_player (Player Pl);

#endif 
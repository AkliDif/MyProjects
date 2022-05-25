#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Pile.h"
#include "Plateau.h"
#include "Player.h"
#include "Tuile.h"


Player* create_player (unsigned int num)
{
    Player* Joueur = (Player*)malloc(sizeof(Player));
    printf ("Joueur %d : Entrer votre pseudo > ", num);
    scanf ("%s", Joueur->Pseudo);
    
    Joueur->Num = num;
    Joueur->Score = 0;
    return Joueur;
}

void print_player (Player Pl)
{
    printf("Num = %d\n", Pl.Num);
    printf("Pseudo = %s\n", Pl.Pseudo);
    printf("Score = %d\n", Pl.Score);
}
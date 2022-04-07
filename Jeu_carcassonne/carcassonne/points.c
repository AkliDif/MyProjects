#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Meeple.h"
#include "Pile.h"
#include "Plateau.h"
#include "Player.h"
#include "Tuile.h"
#include "Tuile.c"

int hasMeeple(Tuile T){
    if (T.contient_meeple==1){
        return 1;
    }
    else
        return 0;
}


void find_tuile(int *x, int *y, Tuile** plateau, Tuile* tuile){
    for (x=0; x<72; x++){
        for (y=0; y<72; y++){
            if (plateau[*x][*y].nb == tuile->nb)
                return;
        }
    }
}

int isCompleted(Tuile** plateau, Tuile* tuile, char cote[1]){
    int *x, *y;
    int i, j, nb_tuiles=0;

    Tuile *cur_cote = tuile->cote_A;
    if (cote=='A') cur_cote = tuile->cote_A;
    if (cote=='B') cur_cote = tuile->cote_B;
    if (cote=='C') cur_cote = tuile->cote_C;
    if (cote=='D') cur_cote = tuile->cote_D;
    if (cote=='E') cur_cote = tuile->cote_E;

    find_tuile(x, y, plateau, tuile);           // on localise la tuile dont la structure est à tester
    
    if (cur_cote=="abbaye"){
        for (i=-1 ; i<=1 ; i++){
            for (j=-1 ; j<=1 ; j++){
                if (plateau[i][j].vide!=1) nb_tuiles++;
            }
        }
        if (nb_tuiles==8) return 1;
        else return 0;
    }

    else if (cur_cote=="route"){
        //...
    }


//...
}



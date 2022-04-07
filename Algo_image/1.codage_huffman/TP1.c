#include <stdio.h>
#include <stdlib.h>
#include "TP1.h"


unsigned int stringLength(char *string)
{
    unsigned int size = 0;
    while (string[size] != '\0')
    {
        size++;
    }
    return size;
}



unsigned int charOccurenceInString(char c, char *string)
{
    unsigned int occ=0, i=0;
    while (string[i] != '\0')
    {
        if (string[i] == c)
            occ++;
        i++;
    }
    return occ;
}


char *getStringFromFile(char *filename)
{
    char* string;
    char c;
    unsigned int size = 0, i=0;
    FILE * f = fopen (filename, "r");
    if (f == NULL)
    {
        fprintf (stderr, "Error. Ouverture de fichier echouée\n");
        exit (1);
    }

    while (fgetc(f) != EOF)
    {
        size++;
    }
    printf ("size = %d\n", size);

    
    string = (char*)malloc (size * sizeof(char));
    

    fseek (f, 0, SEEK_SET);
    c = (char)fgetc(f);
    while (c != EOF)
    {
        string[i] = c;
        i++;
        c = (char)fgetc(f);
    }
    string[i-1] = '\0';
    fclose (f);
    
    
    return string;
}



unsigned int nbDifferentChar(char *string)
{
    
    unsigned int i=0, j=0, res=0, n, flag;
    char current_character;
    n = stringLength(string);
    for (i=0 ; i<n; i++)
    {
        flag = 1;
        current_character = string[i];
        for (j=0 ; j<i ; j++)
        {
            if ( current_character == string[j])
            {
                flag=0;
                break;
            }
        }
        res+=flag;
    }
    return res;
}


void printCouple(couple a)
{
    if (a.c == '\n')
        printf ("( \\n, %d)\n", a.occ);
    else 
        printf ("( %c, %d)\n", a.c, a.occ);
}


couple *getOccurencesList(char *string, unsigned int *n)
{
    couple* OccurenceList = (couple*)malloc ((*n) * sizeof(couple));
    if (OccurenceList == NULL)
    {
        fprintf (stderr, "Error : getOccurenceList : malloc echoué !\n");
        exit (1);
    }
    unsigned int i=0, j=0, len, flag, cpt=0;
    char current_character;
    len = stringLength(string);
    for (i=0 ; i<len; i++)
    {
        flag = 1;/* condition */
        current_character = string[i];
        for (j=0 ; j<i ; j++)
        {
            if ( current_character == string[j])
            {
                flag=0;
                break;
            }
        }
        if (flag == 1)
        {
            OccurenceList[cpt].c = current_character;
            OccurenceList[cpt].occ = charOccurenceInString (current_character, string);
            cpt++;
        }
    }

    return OccurenceList;
}

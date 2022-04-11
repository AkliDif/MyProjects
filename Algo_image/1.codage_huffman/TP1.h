#ifndef __TP1_H__
#define __TP1_H__


struct couple_t
{
char c;
unsigned int occ;
};
typedef struct couple_t couple;


unsigned int stringLength(char *string);
unsigned int charOccurenceInString(char c, char *string);
char *getStringFromFile(char *filename);
unsigned int nbDifferentChar(char *string);
void printCouple(couple a);
couple *getOccurenceList(char *string, unsigned int *n);




#endif
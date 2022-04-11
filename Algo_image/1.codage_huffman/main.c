#include <stdio.h>
#include <stdlib.h>
#include "TP1.h"
#include "TP2.h"



int main(int argc, char* argv[])
{
  if(argc < 2)
    {
      fprintf(stderr,"Erreur : le programme doit prendre un fichier texte en paramètre. Par exemple :\n./main fichier_texte.txt\n");
      return 1;
    }
  
  char *string = getStringFromFile(argv[1]);

  printf("\nMessage à compresser :\n\n%s\n",string);

  unsigned int n = 0;
  couple *list = getOccurenceList(string, &n);
  
  sortOccurencesList(list, n);
  
  printf("\nTable des fréquences triée : \n");
  for(unsigned int i = 0 ; i < n ; i++)
    {
      printCouple(list[i]);
      printf("\n");
    }

  node *root = buildHuffmanTree(list,n);

  printf("\nAffichage de l'arbre d'Huffman :\n\n");
  printTreePre(root);

  code *codeList = buildCodeTable(list, n, root);

  printf("\nTable des symboles : \n");
  for(unsigned int i = 0 ; i < n ; i++)
    {
      if(codeList[i].c == '\n')
	printf("\\n : ");
      else
	printf("%c : ", codeList[i].c);
      printf("%s\n", codeList[i].code);
    }
  
  char *stringCompress = compressString(string, codeList, n);
  printf("\nMessage compressé :\n\n%s\n",stringCompress);

  printf("\nLongueur de la compression : %u bits\n",stringLength(stringCompress));
  printf("Longueur du message initial : %ux7 = %u bits\n",stringLength(string), stringLength(string)*7);

  printf("\nTaux de compression : %.3lf\n\n",((double)stringLength(stringCompress))/(stringLength(string)*7));

  /* On libère toute la mémoire allouée restante */
  
  free(string);
  free(list);
  freeTree(root);

  for(unsigned int i = 0 ; i < n ; i++)
    free(codeList[i].code);
  free(codeList);

  free(stringCompress);
  
  return 0;
}
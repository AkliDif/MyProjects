#include "stdio.h"
#include "stdlib.h"
#include "TP1.h"
#include "TP2.h"



couple *getOccurenceList(char* string, unsigned int *n)
{
  /* On compte le nombre de caracères différents présents dans la chaine */
  *n = nbDifferentChar(string);

  /* On alloue un tableau de couple pour stocker le résultat */
  couple *result = (couple *) malloc((*n)*sizeof(couple));
  if (result == NULL)
    {
      fprintf(stderr,"Error getOccurenceList : malloc failed!\n");
      exit(1);
    }

  int cpt = 0;
  unsigned int lengthString = stringLength(string);
  for(int i = 0 ; i < lengthString ; i++)
    {
      char currentChar = string[i];
      /* On commence par vérfier que l'on a pas déjà recontré le caractère courant */
      int flag = 1;
      for(int j = 0 ; j < i ; j++)
	{
	  if(currentChar == string[j])
	    {
	      flag = 0;
	      break;
	    }
	}
      /* Si c'est un caractère que l'on a pas déjà rencontré, on le rajoute à la liste avec son nombre d'occurences. */
      if(flag == 1)
	{
	  result[cpt].c = string[i];
	  result[cpt].occ = charOccurenceInString(result[cpt].c,string);
	  cpt++;
	}
    }
  
  return result;
}


node *createNode(Tval key, node *left, node *right)
{
  node *res = (node*) malloc(sizeof(node));

  if( res == NULL)
    {
      fprintf(stderr,"Erreur createNode : malloc failed !\n");
      exit(1);
    }

  res->key = key;
  res->left = left;
  res->right = right;

  return res;
}


void freeTree(node *root)
{
  /* Dans ce cas, rien à faire */
  if(root == NULL)
    return;

  /* On libère récursivement les fils gauche et droit avant de libérer le noeud courant. */
  freeTree(root->left);
  freeTree(root->right);
  
  free(root);
}

void printTreePre(node *root)
{
  if(root == NULL)
    return;

  /* On commence par imprimer le noeud courant */
  if(root->left == NULL && root->right == NULL )
    {
      printf("{");
      printCouple(root->key);
      printf(", NULL, NULL}\n");
      return;
    }

  if(root->right == NULL)
    {
      printf("{");
      printCouple(root->key);
      printf(", ");
      printCouple(root->left->key);
      printf(", NULL}\n");
    }
  else if(root->left == NULL)
    {
      printf("{");
      printCouple(root->key);
      printf(", NULL, ");
      printCouple(root->right->key);
      printf("}\n");
    }
  else
    {
      printf("{");
      printCouple(root->key);
      printf(", ");
      printCouple(root->left->key);
      printf(", ");
       printCouple(root->right->key); 
      printf("}\n");
    }
  /* Puis on imprime récursivement les fils gauche et droit */
  printTreePre(root->left);
  printTreePre(root->right);
}

unsigned int depth(node *root)
{
  if(root == NULL)
    return 0;
  
  if(root->left == NULL && root->right == NULL)
    return 0;

  unsigned int depthLeft = 1 + depth(root->left);
  unsigned int depthRight = 1 + depth(root->right);

  if(depthLeft < depthRight )
    return depthRight;

  return depthLeft;
}

int getCode(char c, node *root, char code[], unsigned int *n)
{
  if(root == NULL)
    return 0;
  
  if(root->key.c == c)
    {
      code[*n] = '\0';
      return 1;
    }
  (*n)++;
  
  code[*n-1] = '0';
  if(getCode(c,root->left,code,n) == 1)
      return 1;

  code[*n-1] = '1';
  if(getCode(c,root->right,code,n) == 1)
    return 1;

  (*n)--;
  return 0;
}

/************** Deuxième partie du TP2 **************/

/* Tri à bulles classique */
void sortOccurencesList(couple *list, unsigned int n)
{
  for(unsigned int i = 0 ; i < n-1 ; i++)
      for(unsigned int j = 0 ; j < n-1-i ; j++)
	{
	  if( list[j].occ > list[j+1].occ)
	    {
	      couple tmp = list[j];
	      list[j] = list[j+1];
	      list[j+1] = tmp;
	    }
	}
}

node *buildHuffmanTree(couple *list, unsigned int n)
{
  /* On alloue des noeuds pour chaque couple de la liste */
  node ** listNodes = (node**) malloc(n*sizeof(node*));
  if(listNodes == NULL)
    {
      fprintf(stderr,"Erreur buildHuffmanTree : first malloc failed!\n");
      exit(1);
    }
  for(unsigned int i = 0 ; i < n ; i++)
      listNodes[i] = createNode(list[i], NULL, NULL);  

  /* On itère le procédé tant qu'on a plus d'un élément dans le tableau */
  while ( n > 1 )
    {      
      /* On crée le noeud père des deux premiers éléments de la liste */
      unsigned int sumOcc = listNodes[0]->key.occ + listNodes[1]->key.occ;
      couple coupleTmp = {'\0',sumOcc};
      node *nodeTmp = createNode(coupleTmp, listNodes[0], listNodes[1]);

      /* On regarde où insérer le noeud que l'on vient de créer sans modifier l'ordre du tri */
      unsigned int indexInsertion = 0;
      while( sumOcc > listNodes[indexInsertion+2]->key.occ && indexInsertion < n-2)
	indexInsertion++;

      /* On décale de deux crans tous les noeuds d'indice inférieur à indexInsertion */
      for(unsigned int j = 2 ; j < indexInsertion+2 ; j++)
	listNodes[j-2] = listNodes[j];
      /* On insère le nouveau noeud dans la liste */
      listNodes[indexInsertion] = nodeTmp;
      /* On décale d'un cran tous les noeuds d'indice supérieur à indexInsertion */
      for(unsigned int j = indexInsertion+2 ; j < n ; j++)
	listNodes[j-1] = listNodes[j]; 
      
      /* On décrémente la taille de la liste */
      n--;
    }

  /* Le résultat est le premier élément du tableau */
  node *res = listNodes[0];

  /* On libère la mémoire de listNodes. Remarquez que la mémoire des éléments de listNodes est elle toujours allouée et qu'on peut toujours accéder à chacun des noeuds créés via res. */
  free(listNodes);
  
  return res;
}

code *buildCodeTable(couple *list, unsigned int n, node *root)
{

  
  code *res = (code*) malloc(n*sizeof(code));
  if(res == NULL)
    {
      fprintf(stderr,"Erreur buildCodeTable : malloc failed!\n");
      exit(1);
    }

  char Code[depth(root)+1];
  for(unsigned int i = 0 ; i < n ; i++)
    {
      unsigned int tailleCode = 0;
      getCode(list[i].c, root, Code, &tailleCode);

      res[i].c = list[i].c;
      res[i].code = (char*) malloc((tailleCode+1)*sizeof(char)); // On rajoute un élément pour avoir le caractère de fin de chaîne.
      if(res[i].code == NULL)
	{
	  fprintf(stderr, "Erreur buildCodeTable : malloc of res[%d] failed!\n",i);
	  exit(1);
	}
      for(int j = 0 ; j < tailleCode ; j++)
	res[i].code[j] = Code[j];
      res[i].code[tailleCode] = '\0';

      res[i].n = tailleCode;
    }

  return res;
}

char *compressString(char *string, code *codeList, unsigned int n)
{
  /* On commence par calculer la longueur de chaîne nécessaire pour le résultat */
  unsigned int lengthInput = stringLength(string), lengthOutput = 0;

  for(unsigned int i = 0 ; i < lengthInput ; i++)
    {
      /* On cherche le code du caractère d'indice i de l'entrée */
      unsigned int indexSymbole = 0;
      while(codeList[indexSymbole].c != string[i])
	indexSymbole++;
      
      lengthOutput += codeList[indexSymbole].n;
    }
  
  /* On alloue la mémoire du résultat en rajoutant un caractère pour le caractère de fin de chaîne */
  char *res = (char*) malloc((lengthOutput+1)*sizeof(char));
  if( res == NULL)
    {
      fprintf(stderr,"Erreur compressString : malloc failed!\n");
      exit(1);
    }

  /* On copie le résultat dans la nouvelle chaîne */
  unsigned int index = 0;
  for(unsigned int i = 0 ; i < lengthInput ; i++)
    {
      /* On cherche le code du caractère d'indice i de l'entrée */
      unsigned int indexSymbole = 0;
      while(codeList[indexSymbole].c != string[i])
	indexSymbole++;

      /* On copie le code correspondant au caractère d'indice i */
      for(unsigned int j = 0 ; j < codeList[indexSymbole].n ; j++)
	{
	  res[index] = codeList[indexSymbole].code[j];
	  index++;
	}
    }

  res[index] = '\0'; // On termine par le caractère de fin de chaîne.

  return res;
}
#ifndef __TP2_H__
#define __TP2_H__


typedef couple Tval;

struct node_t
{
    couple key; // La clef
    struct node_t *left, *right; // Pointeurs vers les fils gauche et droit
};

typedef struct node_t node;


struct code_t
{
  char c;
  char *code;
  unsigned int n;
};
typedef struct code_t code;



node *createNode(couple key, node *left, node *right);
void freeTree(node *root);
void printTreePre(node *root);
void printTreePost(node *root);
unsigned int nbLeafs(node *root);
unsigned int depth(node *root);
int getCode(char c, node *root, char code[], unsigned int *n);
void sortOccurencesList(couple *list, unsigned int n);
node *buildHuffmanTree(couple *list, unsigned int n);

code *buildCodeTable(couple *list, unsigned int n, node *root);
char *compressString(char *string, code *codeList, unsigned int n);



#endif


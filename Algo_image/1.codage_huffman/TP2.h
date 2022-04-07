#ifndef __TP2_H__
#define __TP2_H__

typedef int Tval;
struct node_t
{
    couple key; // La clef
    struct node_t *left, *right; // Pointeurs vers les fils gauche et droit
};

typedef struct node_t node;


node *createNode(couple key, node *left, node *right);
void freeTree(node *root);
void printTreePre(node *root);
void printTreePost(node *root);
unsigned int nbLeafs(node *root);
unsigned int depth(node *root);
int getCode(char c, node *root, char code[], unsigned int *n);
void sortOccurencesList(couple *list, unsigned int n);
node *buildHuffmanTree(couple *list, unsigned int n);



#endif
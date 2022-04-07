#include <stdio.h>
#include <stdlib.h>
#include "TP1.h"
#include "TP2.h"


node *createNode(couple key, node *left, node *right)
{
    node* N = (node*)malloc(sizeof(node));
    if (N == NULL)
    {
        fprintf (stderr, "Error createNode : malloc echoué\n");
        exit(1);
    }

    N->key = key;
    N->left = left;
    N->right = right;
    return N;
}


void freeTree(node *root)
{
    if (root == NULL)
        return;
        
    freeTree (root->left);
    freeTree (root->right);
    free (root);
}


void printTreePre(node *root)
{
    if (root == NULL)
        return;
    
    else 
    {
        printf ("{");
        printCouple (root->key);
        if (root ->left == NULL)
            printf ("NULL, ");
        else 
            printcouple(root->left->key);
        if (root->right == NULL)
            printf ("NULL}\n");
        else 
           printcouple(root->right->key);
    }
    printTreePre (root->left);
    printTreePre (root->right);
}



void printTreePost(node *root)
{
    if (root == NULL)
        return;

    else 
    {
        printTreePost (root->left);
        printTreePost (root->right);

        printf ("{%d, ", root->key);;
        if (root ->left == NULL)
            printf ("NULL, ");
        else 
            printf ("%d, ", root->left->key);
        if (root->right == NULL)
            printf ("NULL}\n");
        else 
            printf ("%d}\n", root->right->key);
    }
}

unsigned int nbLeafs(node *root)
{   
    unsigned int res = 0;
    if (root == NULL)
        return 0;
    if (root->left != NULL)
        res += nbLeafs (root->left);
    if (root->right != NULL)
        res += nbLeafs (root->right);
    res+=1;
    return res;
}


unsigned int depth(node *root)
{
    unsigned int res1 = 0, res2 = 0;
    if (root == NULL)
        return 0;
    if (root->left != NULL)
        res1 = 1 + depth (root->left);
    if (root->right != NULL)
        res2 = 1 + depth (root->right);

    if (res1 > res2)
        return res1;
    return res2;
}

int getCode(char c, node *root, char code[], unsigned int *n)
{
    if (root == NULL)
        return 0;
    if (root->key.c == c)
    {
        code[*n] = '\0';
        return 1;
    }

    (*n)++;
    code [*n-1] = '0';
    if (getCode (c, root->left, code, n) == 1)
        return 1;
    
    code[*n-1] = '1';
    if (getCode(c, root->right, code, n) == 1)
        return 1;
    
    (*n)--;

    return 0;
}


void sortOccurencesList(couple *list, unsigned int n)
{
    int i, j;
    couple temp;

    for (i=0 ; i < n-1 ; i++)
    {
        for (j=i+1 ; j<i ; j++)
        {
            if (list[i].occ > list[j].occ)
            {
                temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
}


node *buildHuffmanTree(couple *list, unsigned int n)
{
    int i = 0;
    node** listeNodes = (node**)malloc (n * sizeof(node*));
    for (i=0 ; i<n ; i++)
        listeNodes[i] = createNode(list[i], NULL, NULL);
    
    couple temp;
    node* Nodetemp;
    while (n>1)
    {
        temp.occ = listeNodes[0]->key.occ + listeNodes[1]->key.occ;
        temp.c = '\0';
        Nodetemp = createNode (temp, listeNodes[0], listeNodes[1]);

        for (i=0 ; i<n ; i++)
        {
            if (Nodetemp->key.occ <= listeNodes[i]->key.occ)
        }
    }
    
}
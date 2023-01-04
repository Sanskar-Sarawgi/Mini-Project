#define MAX_CHAR 26
#define WORD_SIZE 20
#include "Buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <NRank.h>

typedef struct Trie
{
    int count;
    struct Trie *node_list[MAX_CHAR];
} Trie;

char lower(char );

int is_char(char );

Trie *Create();

Trie *Insert(Trie *, char *, int );

Trie *Build_Trie(Buffer_node *);

int Frequence_Of_Word(FILE *, Trie *, char *, int );

int Find_freq(Trie *, char *);

void Frequence_Of_N_Word(Contaner *, Trie *, char *, int );

Contaner *Find_N_freq(Trie *, int );
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 26
#define WORD_SIZE 20

typedef struct Trie
{
    // count of word
    // array with label the branch
    int count;
    struct Trie *node_list[MAX_CHAR];
} Trie;

// Convert the Upper char to lower char
char lower(char ch)
{
    if (ch >= 65 && ch <= 90)
        return ch + 32;
    return ch;
}

// check input is character => 1 or (Special char, Symbol, digit) => 0
int is_char(char ch)
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        return 1;
    return 0;
}

// create Trie node using malloc
Trie *Create()
{
    Trie *node = (Trie *)malloc(sizeof(Trie));
    for (int i = 0; i < MAX_CHAR; i++)
        node->node_list[i] = NULL;
    node->count = 0;
    return node;
}

Trie *Insert(Trie *head, char *str, int len)
{
    if (head == NULL)
        head = Create();
    Trie *temp = head;
    for (int i = 0; i < len; i++)
    {
        int index = lower(str[i]) - 'a'; // converting char to index like a=0 , b=1 , c=2 ...

        // if branch not created
        if (!temp->node_list[index])
        {

            temp->node_list[index] = Create();
        }

        temp = temp->node_list[index]; // move to the next node respect of the char or index
    }

    // incress the count of the word to find the frequency
    temp->count++;
    return head;
}

Trie *Build_Trie(Buffer_node *Contant_data)
{
    Cursor *cur = Create_Iterater(Contant_data);
    int temp;
    Trie *Node = NULL;
    do
    {
        char word[WORD_SIZE + 4];
        int len = 0;
        temp = Read_buffer(cur);
        while (is_char(temp))
        {
            word[len++] = temp;
            if (len > WORD_SIZE)
                break; // limit the word size
            temp = Read_buffer(cur);
        }

        if (len > WORD_SIZE)
            continue;
        word[len] = '\0';

        if (len > 0)
            Node = Insert(Node, word, len);
        // Checking if character is not EOF.
        // If it is EOF stop reading.
    } while (temp != EOF);

    return Node;
}

int Frequence_Of_Word(FILE *out_file, Trie *head, char *word, int index)
{
    static int counter = 0;

    if (!head)
        return 0;
    if (head->count > 0)
    {
        fprintf(out_file, "%s = %d \n", word, head->count);
        counter++;
    }
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (head->node_list[i])
        {
            word[index + 1] = 'a' + i;
            word[index + 2] = '\0';
            Frequence_Of_Word(out_file, head->node_list[i], word, index + 1);
            word[index + 1] = '\0';
        }
    }
    return counter;
}

int Find_freq(Buffer_node *Contant_data, char *result_name)
{

    FILE *out_file = fopen(result_name, "w");
    Trie *node = Build_Trie(Contant_data);
    int n = Frequence_Of_Word(out_file, node, (char *)calloc(50, sizeof(char)), -1);
    fclose(out_file);
    return n;
}

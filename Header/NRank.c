#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MIN_VAL -100

typedef struct Word_Node
{
    char name[20];
    int fre;
} Word_Node;

typedef struct Contaner
{
    Word_Node *list;
    int size;
} Contaner;

Contaner *Create_Container(int size)
{
    Contaner *temp = (Contaner *)malloc(sizeof(Contaner));
    temp->list = (Word_Node *)calloc(size+1, sizeof(Word_Node));
    for (int i = 0; i < size; i++)
        temp->list[i].fre = INT_MIN_VAL;
    temp->size = size;
    return temp;
}

void Add_Element(char *name, int fre, Contaner *cnt)
{
    if (cnt == NULL)
        return;

    Word_Node *temp = cnt->list;
    int index = cnt->size - 1;

    //printf("Debug\n"); 
    if (fre > temp[index].fre)
    {
        while (index >= 0)
        {
            if (temp[index].fre < fre)
            {
                temp[index + 1].fre = temp[index].fre;
                strcpy(temp[index + 1].name, temp[index].name);
            }
            else
                break;
            index--;
        }
        temp[index+1].fre = fre;
        strcpy(temp[index+1].name, name);
    }
}

int Last_Index(Contaner *cnt){
    int index = -1;
    for(int i = 0 ;i < cnt->size;i++){
        if(cnt->list[i].fre == INT_MIN_VAL) break;
        index++;
    }
    return index;
}

void Print_Element(Contaner *cnt)
{
    if (cnt == NULL)
        return;

    int last_in = Last_Index(cnt);
    printf("|%-15s | %s|\n", "Word", "freq");
    printf("+----------------------+\n");
    for (int i = 0; i <= last_in; i++)
    {
        printf("|%-15s | %3d |\n", cnt->list[i].name, cnt->list[i].fre);
    }
    printf("+----------------------+\n");
}

void Clear_Container(Contaner *cnt){
    free(cnt->list);
    free(cnt);
}

// int main()
// {
//     char *name[] = {"Word1", "Word2", "Word3", "Word4", "Word5", "Word6", "Word7"};
//     int fre[] = {10, 2, 5, 21, 100, 45, 9};
//     Contaner *cnt = Create(3);
//     for (int i = 0; i < 7; i++)
//     {
//         Add_Element(name[i], fre[i], cnt);
//     }
//     //printf("%d",Last_Index(cnt));
//     Print_Element(cnt);
//     return 1;
// }
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int fre;
    char *word;
} node;

int cmpfunc(const void *a, const void *b)
{
    return (((node *)b)->fre - ((node *)a)->fre);
}

void Sort_file(int len, char *file_name)
{
    node value[len];
    FILE *in_file = fopen(file_name, "r");

    // input from the value
    for (int i = 0; i < len; i++)
    {
        char *temp = (char *)calloc(20, sizeof(char));
        int j = 0;
        char ch = fgetc(in_file);
        while (ch != ' ')
        {
            temp[j++] = ch;
            ch = fgetc(in_file);
        }
        value[i].word = temp;
        ch = fgetc(in_file); // =
        ch = fgetc(in_file); // empty
        char num = fgetc(in_file);
        int number = 0;
        while (num != ' ')
        {
            number *= 10;
            number += (num - '0');
            num = fgetc(in_file);
        }
        value[i].fre = number;
        fgetc(in_file); // /n
    }

    qsort(value, len, sizeof(node), cmpfunc);

    fclose(in_file);
    FILE *out_file = fopen(file_name, "w");
    fprintf(out_file, "|%-15s | %s|\n", "Word", "freq");
    fprintf(out_file, "+----------------------+\n");
    for (int n = 0; n < len; n++)
    {
        fprintf(out_file, "|%-15s | %3d |\n", value[n].word, value[n].fre);
    }
    fprintf(out_file, "+----------------------+\n");
    fclose(out_file);
}
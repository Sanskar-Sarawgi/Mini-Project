typedef struct node
{
    int fre;
    char *word;
} node;

int cmpfunc(const void *, const void *);

void Sort_file(int , char *);
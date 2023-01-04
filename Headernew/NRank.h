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

Contaner *Create_Container(int );

void Add_Element(char *, int , Contaner *);

int Last_Index(Contaner *);

void Print_Element(Contaner *);

void Clear_Container(Contaner *);
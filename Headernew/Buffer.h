typedef struct Buffer_node
{
    char *para;
    int size;
    struct Buffer_node *next;
} Buffer_node;

typedef struct Cursor
{
    Buffer_node *head;
    char position;
    int temp_buff[10];
    int top;
} Cursor;

Buffer_node *Create_Buffer(char *, int );

int string_len(char *);

int string_length(char *);

Buffer_node *Add_buffer(char *, Buffer_node *);

int Read_buffer(Cursor *);

void Free_Node(Buffer_node *);

void Free_list(Buffer_node *);

void Unread_buffer(Cursor *, int );

void Print(Buffer_node *);
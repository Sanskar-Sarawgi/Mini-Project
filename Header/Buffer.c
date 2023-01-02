#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Cursor *Create_Iterater(Buffer_node *node)
{
    Cursor *cur = (Cursor *)malloc(sizeof(Cursor));
    cur->head = node;
    cur->position = 0;
    cur->top = -1;
    return cur;
}

Buffer_node *Create_Buffer(char *ch, int size)
{
    Buffer_node *ptr = (Buffer_node *)malloc(sizeof(Buffer_node));
    ptr->para = ch;
    ptr->size = size;
    ptr->next = NULL;
    return ptr;
}

int string_len(char *ch)
{
    int i = 0;
    if (ch[i++] != '\0')
        ;
    return i;
}

int string_length(char *ch)
{
    int i = 0;
    while (ch[i] != '\0' && i < 100)
    {
        i++;
    }
    return i;
}

// add paragraph in the node
Buffer_node *Add_buffer(char *buff, Buffer_node *head)
{
    int len = string_length(buff);
    char *para = calloc(len, sizeof(char));
    strcpy(para, buff);
    if (head == NULL)
    {
        head = Create_Buffer(para, len);
    }
    else
    {
        Buffer_node *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = Create_Buffer(para, len);
    }
    return head;
}

// reading function
int Read_buffer(Cursor *cur)
{
    if (cur->head == NULL)
        return EOF;
    if (cur->top >= 0)
    {
        return cur->temp_buff[(cur->top)--];
    }
    if (cur->position >= cur->head->size)
    {
        cur->head = cur->head->next;
        cur->position = 0;
    }
    if (cur->head == NULL)
        return EOF;
    int ch = cur->head->para[cur->position];
    cur->position++;
    return ch;
}

void Free_Node(Buffer_node *head)
{
    free(head->para);
    free(head);
}

void Free_list(Buffer_node *head)
{
    if (head == NULL)
        return;
    Free_list(head->next);
    Free_Node(head);
}

void Unread_buffer(Cursor *cur, int ch)
{
    if (cur->top >= 10)
        return;
    cur->top++;
    cur->temp_buff[cur->top] = ch;
}

void Print(Buffer_node *contant)
{
    Cursor *cur = Create_Iterater(contant);
    int temp = Read_buffer(cur);
    while (cur->head)
    {
        printf("%c", temp);
        temp = Read_buffer(cur);
    }
}

// int main(){
//     char ch[100];
//     Buffer_node* contant = NULL;
//     int temp;

//     FILE *in_file = fopen("./Html_Page.html", "r");
//     while(temp != EOF){
//         int i = 0;
//         temp = fgetc(in_file);
//         while(temp != EOF && i < 98){
//             ch[i++] = temp;
//             temp = fgetc(in_file);
//         }
//         ch[i] = '\0';
//         contant=Add_buffer(ch,contant);
//     }
//     fclose(in_file);
//     test(contant);
// }

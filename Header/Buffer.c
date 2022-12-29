#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Buffer_node{
    char *para;
    int size;
    struct Buffer_node* next;
}Buffer_node;

typedef struct Cursor{
    Buffer_node* head;
    char position;
}Cursor;

Cursor* Create_Iterater(Buffer_node* node){
    Cursor* cur = (Cursor *)malloc(sizeof(Cursor));
    cur->head = node;
    cur->position = 0;
    return cur;
}

Buffer_node* Create_Buffer(char *ch,int size){
    Buffer_node* ptr = (Buffer_node *)malloc(sizeof(Buffer_node));
    ptr->para = ch;
    ptr->size = size;
    ptr->next = NULL;
    return ptr;
}

int string_len(char *ch){
    int i = 0;
    if(ch[i++] != '\0');
    return i;
}


// add paragraph in the node
Buffer_node* Add_buffer(char* buff,Buffer_node* head){
    int len = strlen(buff);
    char *para = calloc(len,sizeof(char));
    strcpy(para,buff);
    if(head == NULL){
        head = Create_Buffer(para,len);
    }
    else{
        Buffer_node* temp = head;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = Create_Buffer(para,len);
    }
    return head;
}

// reading function
int Read_buffer(Cursor* cur){
    if(cur->head == NULL) return EOF;
    if(cur->position >= cur->head->size){
        cur->head = cur->head->next;
        cur->position = 0;
    }
    if(cur->head == NULL) return EOF;
    int ch = cur->head->para[cur->position];
    cur->position++;
    return ch;
}

void Free_Node(Buffer_node* head){
    free(head->para);
    free(head);
}

void Free_list(Buffer_node* head){
    if(head == NULL) return;
    Free_list(head->next);
    Free_Node(head);
}


// Testing
void test(Buffer_node* contant){
    Cursor* cur = Create_Iterater(contant);
    int temp = Read_buffer(cur);
    while(cur->head){
        printf("%c \n",temp);
        temp = Read_buffer(cur);
    }
    printf("next\n");
}

// int main(){
//     char ch[100];
//     Buffer_node* contant = NULL;
//     for(int i=0;i<3;i++){
//         scanf("%[^\n]%*c", ch);
//         contant=Add_buffer(ch,contant);
//     }
//     test(contant);
// }

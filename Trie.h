#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 26

typedef struct Trie
{
    // count of word
    // array with label the branch
    int count;
    struct Trie *node_list[MAX_CHAR];
}Trie;

char lower(char ch){
    if(ch >= 65 && ch <= 90) return ch+32;
    return ch;
}

int is_char(char ch){
    if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) return 1;
    return 0;
}

Trie *Create(){
    Trie*  node= (Trie *)malloc(sizeof(Trie));
    for(int i=0; i<MAX_CHAR; i++)
        node->node_list[i] = NULL;
    node->count = 0;
    return node;
}


Trie* Insert(Trie* head,char *str,int len){
    if(head == NULL) head = Create();
    Trie* temp = head;
    for(int i=0;i<len;i++){
        int index = lower(str[i]) - 'a';
        if (!temp->node_list[index]){

            temp->node_list[index] = Create();
        }
 
        temp = temp->node_list[index];
    }
    temp->count++;
    return head;
}

Trie *Build_Trie(char *File_Name){

    FILE *in_file = fopen(File_Name, "r");
    int temp;
    Trie *Node = NULL;
    do {
        char word[20];
        int len = 0;
        temp = fgetc(in_file);
        while(is_char(temp)){
            word[len++] = temp;
            temp = fgetc(in_file);   
        }
        word[len] = '\0';
        if(len > 0) Node = Insert(Node,word,len);
        // Checking if character is not EOF.
        // If it is EOF stop reading.
    }while (temp != EOF);

    return Node;
}

void *Frequence_Of_Word(char *File_Name,Trie* head,char* word,int index){
    static FILE *out_file;
    if(out_file == NULL) out_file = fopen(File_Name, "w");
    
    
    if(head->count > 0){
        fprintf(out_file,"%s = %d \n",word,head->count);
    }
    for(int i=0;i<MAX_CHAR;i++){
        if(head->node_list[i]){
            
            word[index+1] = 'a'+i;
            word[index+2] = '\0';
            Frequence_Of_Word(File_Name,head->node_list[i],word,index+1);
            word[index+1] = '\0';
        }
    }
}

// int main(){
//     Trie* node = Build_Trie("test.txt");
//     char word[50];
//     Frequence_Of_Word("./Result/output.txt",node,word,-1);
//     return 0;
// }
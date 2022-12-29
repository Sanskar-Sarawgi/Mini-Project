#include "stdio.h"

char lower(char);

int Is_Tag(FILE *in_file,char *tag){
	int j = 0;
	int temp = fgetc(in_file);
	int result = 0;
	int len = strlen(tag);
	while(temp != EOF && temp != '>'){
		if(tag[j] == lower(temp)) j++;
		else if (temp == ' '); // ignore the empty space between the word
		else j = 0; // if the charater not found for a word we reset the pointer index to start
		if (j == len){
			result = 1;
			break;
		}
		temp = fgetc(in_file);
	}
	while(temp != EOF && temp != '>') temp = fgetc(in_file);
	ungetc(temp, in_file);
	return result;
}

void Blocking(FILE *in_file,char* tag){
	int temp = fgetc(in_file);
	while(temp != EOF){
		if(temp == '<' && Is_Tag(in_file,tag)) break;
		temp = fgetc(in_file);
	}
	if(temp == EOF) ungetc(temp, in_file);
}

void Remove_Tag(char *Html_file, char *result_file)
{
	// Opening file in reading mode
	FILE *in_file = fopen(Html_file, "r");
	FILE *out_file = fopen(result_file, "w");

	// find body in html
	int temp = fgetc(in_file);
	while(temp != EOF){
		if(temp == '<' && Is_Tag(in_file,"body")){
			break;
		}
		temp = fgetc(in_file);
	}

	do
	{
		if (temp == '<')
		{ // starting of tag
		   if(Is_Tag(in_file,"script")){
			   Blocking(in_file,"/script");
		   }
		} // ending of tag
		else if(temp == '>') ;
		else
			fputc(temp, out_file);
		temp = fgetc(in_file);
		// Checking if character is not EOF.
		// If it is EOF stop reading.
	} while (temp != EOF);
	// close file stream
	fclose(out_file);
	fclose(in_file);
}

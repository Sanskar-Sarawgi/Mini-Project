#include "stdio.h"

char lower(char);

void Remove_Tag(char *Html_file,char *result_file)
{
	// Opening file in reading mode
	FILE *in_file = fopen(Html_file, "r");
	FILE *out_file = fopen(result_file, "w");

	int temp;

	// find body in html
	char *word = "<body";
	int j = 0;
	while (temp != EOF)
	{
		temp = fgetc(in_file);
		if (word[j] == lower(temp))
			j++;
		else if (temp == ' ')  // ignore the empty space between the word
			;
		else
			j = 0;    // if the charater not found for a word we reset the pointer index to start
		if (j == 5)
			break;
	}

	int block = 1;
	do
	{
		temp = fgetc(in_file);
		if (temp == '<')
		{ // starting of tag
			while (temp != '>')
			{
				temp = fgetc(in_file);
			}
		}
		else
		{
			fputc(temp, out_file);
		}

		// Checking if character is not EOF.
		// If it is EOF stop reading.
	} while (temp != EOF);
	// close file stream
	fclose(out_file);
	fclose(in_file);
}

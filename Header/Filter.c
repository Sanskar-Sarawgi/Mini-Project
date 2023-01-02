#include "stdio.h"

char lower(char);

int Is_Tag(Cursor *cur, char *tag)
{
	int j = 0;
	int temp = Read_buffer(cur);
	int result = 0;
	int len = strlen(tag);
	while (temp != EOF && temp != '>')
	{
		if (tag[j] == lower(temp))
			j++;
		else if (temp == ' ')
			; // ignore the empty space between the word
		else
			j = 0; // if the charater not found for a word we reset the pointer index to start
		if (j == len)
		{
			result = 1;
			break;
		}
		temp = Read_buffer(cur);
	}
	while (temp != EOF && temp != '>')
		temp = Read_buffer(cur);
	Unread_buffer(cur, temp);
	return result;
}

void Blocking(Cursor *cur, char *tag)
{
	int temp = Read_buffer(cur);
	while (temp != EOF)
	{
		if (temp == '<' && Is_Tag(cur, tag))
			break;
		temp = Read_buffer(cur);
	}
	if (temp == EOF)
		Unread_buffer(cur, temp);
}

Buffer_node *Remove_Tag(Buffer_node *Html_data, Buffer_node *Contant_data)
{
	// Opening file in reading mode
	Cursor *cur = Create_Iterater(Html_data);
	char buffer[100];
	int pointer_buffer = 0;
	// find body in html
	int temp = Read_buffer(cur);
	while (temp != EOF)
	{
		if (temp == '<' && Is_Tag(cur, "body"))
		{
			break;
		}
		temp = Read_buffer(cur);
	}

	do
	{
		if (temp == '<')
		{ // starting of tag
			if (Is_Tag(cur, "script"))
			{
				Blocking(cur, "/script");
			}
		} // ending of tag
		else if (temp == '>')
			;
		else
		{
			buffer[pointer_buffer++] = temp;
		}
		temp = Read_buffer(cur);
		if (pointer_buffer == 98 || temp == EOF)
		{
			buffer[pointer_buffer] = '\0';
			Contant_data = Add_buffer(buffer, Contant_data);
			pointer_buffer = 0;
		}
		// Checking if character is not EOF.
		// If it is EOF stop reading.
	} while (temp != EOF);
	return Contant_data;
}

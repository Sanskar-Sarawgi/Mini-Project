#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "./Header/NRank.c"
#include "./Header/Buffer.c"
#include "./Header/Fetch.c"

#define Output_file "./Result/output.txt"

int main(int argc, char **argv)
{

	// argv store name of exe file at index 0 and start from index 1
	if (argc < 2)
	{
		printf("Plese enter domain name...");
		return -1;
	}

	// init of the Result folder
	int check = mkdir("Result", 0777);

	// for http only
	// char Domain_name[100];
	// char Page_name[100];
	// Break_Address(Domain_name, Page_name, argv[1]);
	// // char* Domain_name = "www.google.com";

	// // convert domain name to ip address
	// char *ip = Domain_to_ip(Domain_name);
	// if (ip == NULL)
	// 	return -1;

	// // creating a tcp connection
	// int sockfd = Create_Tcp_Connection(ip);
	// if (sockfd == -1)
	// 	return -1;

	// // sending request for webpage
	// Send_url_request(Domain_name, sockfd, Page_name);

	// // fetching html from server
	// Recv_HTML(sockfd);

	// for https and http
	Buffer_node *Html_data = fetch(argv[1]);

	// remaining process is same
	printf("Data is Processing ...\n");
	// Extracting the data from html page

	Buffer_node *Contant_data = NULL;
	Contant_data = Remove_Tag(Html_data, Contant_data);
	Free_list(Html_data);

	// finding the frequency of each word
	Trie *Trie_node = Build_Trie(Contant_data);
	int Total_Word = Find_freq(Trie_node, Output_file);
	// Print(Contant_data);
	Free_list(Contant_data);

	// find Top N array
	Contaner *cnt = Find_N_freq(Trie_node, 10);
	Print_Element(cnt);
	Clear_Container(cnt);

	// Rank the word acc to the frequency
	Sort_file(Total_Word, Output_file);
	printf("Data Processing Completed you can check the result in Result folder\n");
	// close the socket
	// close(sockfd);
	// printf("Socket Close\n");

	return 0;
}

// gcc Web_Scraper.c -lssl -lcrypto

// Used following Webside for testing :-
// 1. www.rfc-editor.org/rfc/rfc5246
// 2. www.example.com
// time for i in {1..10}; do ./a.out www.rfc-editor.org/rfc/rfc5246; done;

// ISSUE IN PROGRAM

// use more efficent data structure then trie
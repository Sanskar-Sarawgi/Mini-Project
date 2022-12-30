#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
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
	Buffer_node* Html_data = fetch(argv[1]);
// remaining process is same
    printf("Data is Processing ...\n");
	// Extracting the data from html page

	Buffer_node* Contant_data = NULL;
	Contant_data  = Remove_Tag(Html_data, Contant_data);
	Free_list(Html_data);

	// finding the frequency of each word
	int Total_Word = Find_freq(Contant_data, Output_file);
	Free_list(Contant_data);

	// Rank the word acc to the frequency
	Sort_file(Total_Word, Output_file);
	printf("Data Processing Completed you can check the result in Result folder\n");
	// close the socket
	// close(sockfd);
	// printf("Socket Close\n");

	return 0;
}

// gcc Web_Scraper.c -lssl -lcrypto
// ISSUE IN PROGRAM

// use more efficent data structure then trie
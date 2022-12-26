#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "./Header/fetch.h"

#define Html_file "./Result/Html_Page.html"
#define Contant_file "./Result/Contant.txt"
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
	fetch(argv[1]);

// remaining process is same
	// Extracting the data from html page
	Remove_Tag(Html_file, Contant_file);

	// finding the frequency of each word
	int Total_Word = Find_freq(Contant_file, Output_file);

	// Rank the word acc to the frequency
	Sort_file(Total_Word, Output_file);

	// close the socket
	// close(sockfd);
	// printf("Socket Close\n");

	return 0;
}

// ISSUE IN PROGRAM

// 1. www.google.com    (http = working)
//    www.geeksforgeeks.org (https = not working)
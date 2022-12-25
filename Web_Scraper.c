#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "./Header/Socket.h"

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

	char *Domain_name = argv[1];
	// char* Domain_name = "www.google.com";

	// convert domain name to ip address
	char *ip = Domain_to_ip(Domain_name);
	if (ip == NULL)
		return -1;

	// creating a tcp connection
	int sockfd = Create_Tcp_Connection(ip);
	if (sockfd == -1)
		return -1;

	// sending request for webpage
	Send_url_request(Domain_name, sockfd);

	// fetching html from server
	Recv_HTML(sockfd);

	// Extracting the data from html page
	Remove_Tag("./Result/Html_Page.html");

	// finding the frequency of each word
	int Total_Word = Find_freq("./Result/Contant.txt","./Result/output.txt");

	// Rank the word acc to the frequency
	Sort_file(Total_Word,"./Result/output.txt");

	// close the socket
	close(sockfd);
	printf("Socket Close\n");
	return 0;
}

// ISSUE IN PROGRAM

// ./a.out www.google.com    (http = working)
// ./a.out www.geeksforgeeks.org (https = not working)
// ./a.out www.testingmcafeesites.com/index (navigation to different page not working)
// ./a.out www.testingmcafeesites.com 
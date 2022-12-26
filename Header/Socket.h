#include "stdio.h"
#include <netdb.h>
#include "sys/socket.h"
#include "netinet/in.h"
#include <arpa/inet.h>
#include "string.h"
#include "stdlib.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Trie.h"
#include "Filter.h"
#include "Sort.h"

#define BUF_SIZE 4095
#define BUF_SIZE_INC BUF_SIZE + 1
// 80 beacuse it is Stander for web page
#define PORT_NO 80
#define Header_SIZE 512
#ifndef NULL
#define NULL '\0'
#endif

void Break_Address(char *Domain_name, char *Page_name, char *url)
{
	int i = 0; // index of Domain name
	int j = 0; // index of Page name
	int Switch_flow = 1;
	for (int k = 0; url[k] != '\0'; k++)
	{
		if (url[k] == '/')
		{
			Switch_flow = 0;
			Domain_name[i] = '\0';
		}
		else if (Switch_flow)
		{ // add to the Domain_name
			Domain_name[i++] = url[k];
		}
		else
		{ // add to the Page name
			Page_name[j++] = url[k];
		}
	}
	Page_name[j] = '\0';
}

// converting domain name to ip address
char *Domain_to_ip(char *Domain_name)
{
	// it get the ip address from domain name
	struct hostent *ht = gethostbyname(Domain_name);

	// error check if null it will show error
	if (!ht)
	{
		printf("Host name not present");
		return NULL; // breaking the program
	}

	// it convert the internet address to ascii or representable formate
	char *ip = inet_ntoa(*((struct in_addr *)ht->h_addr_list[0]));
	printf("IP : %s \n", ip);
	// getting host ip address
	return ip;
}

// setting up the socket connection
int Create_Tcp_Connection(char *ip)
{

	int sockfd;
	struct sockaddr_in server_addr;
	int portno = PORT_NO;
	// AF_INET - internet protoal
	// sock_stream - using tcp protocal
	// protocol code (0) - zero for tcp
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("error opening socket\n");
		return -1; // to break the program
	}
	else
	{
		printf("socket opened\n");
	}

	// set the value of server address to zero
	memset((char *)&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	// htons = convet host to network standard bit order
	server_addr.sin_port = htons(portno);
	inet_pton(AF_INET, ip, &(server_addr.sin_addr)); // vice versa of inet_ntoa()

	// creating connection with server
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("failed to connect\n");
		return 0;
	}
	else
	{
		printf("connection successful\n");
	}

	return sockfd;
}

void Send_url_request(char *Domain_name, int sockfd, char *Page_name)
{

	char header[Header_SIZE];
	// creating header to send request to the server
	// GET / HTTP/1.1
	// Host: Domain_name
	snprintf(header, Header_SIZE, "GET /%s HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n", Page_name, Domain_name);
	printf("Header - %s\n", header);

	// sending request to server
	int buffer_size = send(sockfd, header, strlen(header), 0);
	if (buffer_size)
	{
		printf("nr %d (%lu)\n", buffer_size, strlen(header));
	}
}

void Recv_HTML(int sockfd)
{
	char *buf;
	buf = (char *)malloc(BUF_SIZE_INC * sizeof(char));

	memset(buf, 0, BUF_SIZE_INC);
	ssize_t bytes_received = recv(sockfd, buf, BUF_SIZE, 0);

	// open the file to store the html page
	FILE *out_file = fopen("./Result/Html_Page.html", "w");

	// getting datapackge
	while (bytes_received > 0)
	{
		buf[bytes_received] = '\0';

		fprintf(out_file, "%s", buf);
		memset(buf, 0, BUF_SIZE_INC);
		bytes_received = recv(sockfd, buf, BUF_SIZE, 0);
	}

	// close file stream
	fclose(out_file);
}
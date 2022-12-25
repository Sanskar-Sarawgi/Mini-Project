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
#include "./Header/Trie.h"
#include "./Header/Filter.h"
#include "./Header/Sort.h"

#define BUF_SIZE 4095
#define BUF_SIZE_INC BUF_SIZE + 1
#define PORT_NO 80
#define Header_SIZE 512
#ifndef NULL
#define NULL '\0'
#endif


// converting domain name to ip address
char* Domain_to_ip(char* Domain_name){

	struct hostent *ht = gethostbyname(Domain_name);

	// error check if null it will show error
	if(!ht){
		printf("Host name not present");
		return NULL; // breaking the program
	}

	// getting host ip address
	return inet_ntoa(*((struct in_addr *)ht->h_addr_list[0]));
}

// setting up the socket connection 
int Create_Tcp_Connection(char *ip){
	
	int sockfd;
	struct sockaddr_in server_addr;
	int portno = PORT_NO;
	// AF_INET - internet protoal
	// sock_stream - using tcp protocal
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		printf("error opening socket\n");
		return -1; // to break the program
	}else{
		printf("socket opened\n");
	}

	// set the value of server address to zero
	memset((char *) &server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portno);
	inet_pton(AF_INET, ip, &(server_addr.sin_addr) );


    // creating connection with server
	if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
		printf("failed to connect\n");
		return 0;
	}else{
		printf("connection successful\n");
	}
	
	return sockfd;
}

void Send_url_request(char *Domain_name,int sockfd){

	char header[Header_SIZE];
	// creating header to send request to the server
	// GET / HTTP/1.1
	// Host: Domain_name
	snprintf(header, Header_SIZE, "GET / HTTP/1.0\r\nHost: %s\r\n\r\n", Domain_name);
	printf("Header - %s\n", header);

	// sending request to server
	int buffer_size = send(sockfd, header, strlen(header), 0);
	if(buffer_size){
		printf("nr %d (%lu)\n", buffer_size, strlen(header));
	}
}


void Recv_HTML(int sockfd){
	char *buf;
	buf = (char *) malloc(BUF_SIZE_INC * sizeof(char));

	memset(buf, 0, BUF_SIZE_INC);
	ssize_t bytes_received = recv(sockfd, buf, BUF_SIZE, 0);

	// open the file to store the html page
	FILE *out_file = fopen("./Result/Html_Page.html", "w");

	// getting datapackge
	while(bytes_received > 0){
		buf[bytes_received] = '\0';
		
		fprintf(out_file, "%s", buf);
		memset(buf, 0, BUF_SIZE_INC);
		bytes_received = recv(sockfd, buf, BUF_SIZE, 0);
	}

	// close file stream
	fclose(out_file);
}



int main(int argc,char **argv){

	// argv store name of exe file at index 0 and start from index 1
	if(argc < 2){
		printf("Plese enter domain name...");
        return -1;
	}

	// init of the Result folder
	int check = mkdir("Result",0777);

	char* Domain_name = argv[1];
	//char* Domain_name = "www.google.com";

	// convert domain name to ip address
	char *ip = Domain_to_ip(Domain_name);
	if(ip == NULL) return -1;
	
	// creating a tcp connection
	int sockfd = Create_Tcp_Connection(ip);
	if(sockfd == -1) return -1;

	// sending request for webpage
	Send_url_request(Domain_name,sockfd);

	// fetching html from server
	Recv_HTML(sockfd);

	// Extracting the data from html page
	Remove_Tag("./Result/Html_Page.html");

	// finding the frequency of each word
	int Total_Word = Find_freq("./Result/Contant.txt");

	//Rank the word acc to the frequency
	Sort_file(Total_Word);

	// close the socket
	
	return 0;
}

// ./a.out www.google.com
// ./a.out www.geeksforgeeks.org
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include "./Header/Trie.c"
#include "./Header/Filter.c"
#include "./Header/Sort.c"
#define Html_file "./Result/Html_Page.html"
#define Contant_file "./Result/Contant.txt"
#define Output_file "./Result/output.txt"

size_t Abstract_Data(char *buffer, size_t itemsize, size_t nitems, void *ignorethis)
{
    size_t bytes = itemsize * nitems;
    FILE *out_file = fopen("./Result/Html_Page.html", "a");

    for (int i = 0; i < bytes; i++)
    {
        fprintf(out_file,"%c", buffer[i]);
    }

    fclose(out_file);
    return bytes;
}

int main()
{
    CURL *curl = curl_easy_init();

    if (!curl)
    {
        printf("init failed\n");
        return -1;
    }

    FILE *out_file = fopen("./Result/Html_Page.html", "w");
    fclose(out_file);

    // set options it is a type of middleware
    curl_easy_setopt(curl, CURLOPT_URL, "https://en.wikipedia.org/wiki/HTTPS");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Abstract_Data);

    // perform out action
    CURLcode result = curl_easy_perform(curl);

    if (result != CURLE_OK)
    {
        printf("%s", curl_easy_strerror(result));
    }

    // Extracting the data from html page
	Remove_Tag(Html_file, Contant_file);

	// finding the frequency of each word
	int Total_Word = Find_freq(Contant_file, Output_file);

	// Rank the word acc to the frequency
	Sort_file(Total_Word, Output_file);

    curl_easy_cleanup(curl);
    return 0;
}

// gcc  Curl_demo.c -lcurl
// http://www-db.deis.unibo.it/courses/TW/DOCS/w3schools/website/customers.html
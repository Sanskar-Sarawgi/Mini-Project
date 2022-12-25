#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

size_t Abstract_Data(char *buffer, size_t itemsize, size_t nitems, void *ignorethis)
{
    size_t bytes = itemsize * nitems;
    int block = 0;
    for (int i = 0; i < bytes; i++)
    {
        if ('<' == buffer[i])
            block = 1;
        if (!block)
            printf("%c", buffer[i]);
        if ('>' == buffer[i])
            block = 0;
    }
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

    // set options it is a type of middleware
    curl_easy_setopt(curl, CURLOPT_URL, "http://www-db.deis.unibo.it/courses/TW/DOCS/w3schools/website/customers.html");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Abstract_Data);

    // perform out action
    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK)
    {
        printf("%s", curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);
    return 0;
}

// gcc  Curl_demo.c -lcurl
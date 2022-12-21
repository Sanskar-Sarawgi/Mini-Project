#include<curl/curl.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
    CURL *curl = curl_easy_init();

    if(!curl){
        printf("init failed\n");
        return -1;
    }

    // set options it is a type of middleware
    curl_easy_setopt(curl,CURLOPT_URL,"http://www-db.deis.unibo.it/courses/TW/DOCS/w3schools/website/customers.html");


    //perform out action
    CURLcode result = curl_easy_perform(curl);
    if(result != CURLE_OK){
        printf("%s",curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);
    return 0;
}
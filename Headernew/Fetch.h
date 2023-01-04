#include <stdio.h>
#include <stdlib.h>
#include <openssl/bio.h> /* BasicInput/Output streams */
#include <openssl/err.h> /* errors */
#include <openssl/ssl.h> /* core library */
#include <string.h>
#include "Buffer.h"
// #include "Trie.c"
// #include "Filter.c"
// #include "Sort.c"

void Break_Address(char *, char *, char *);

void report_and_exit(const char *);

void init_ssl();

void cleanup(SSL_CTX *, BIO *);

Buffer_node *secure_connect(const char *, char *);

Buffer_node *fetch(char *);
#include <stdio.h>
#include <stdlib.h>
#include <openssl/bio.h> /* BasicInput/Output streams */
#include <openssl/err.h> /* errors */
#include <openssl/ssl.h> /* core library */
#include <string.h>
#include "Socket.c"

#define BuffSize 98

void report_and_exit(const char *msg)
{
  perror(msg);
  ERR_print_errors_fp(stderr);
  exit(-1);
}

void init_ssl()
{
  SSL_load_error_strings();
  // registers the available SSL/TLS ciphers and digests.
  SSL_library_init();
}

void cleanup(SSL_CTX *ctx, BIO *bio)
{
  SSL_CTX_free(ctx);
  BIO_free_all(bio);
}

Buffer_node* secure_connect(const char *hostname, char *page)
{
  char name[BuffSize];
  char request[BuffSize];
  char response[BuffSize];

  const SSL_METHOD *method = TLS_client_method();  // confi based on version
  if (NULL == method)
    report_and_exit("TLSv1_2_client_method...");

  SSL_CTX *ctx = SSL_CTX_new(method);  // return a token
  if (NULL == ctx)
    report_and_exit("SSL_CTX_new...");

  BIO *bio = BIO_new_ssl_connect(ctx);  // bio socket I/O
  if (NULL == bio)
    report_and_exit("BIO_new_ssl_connect...");

  SSL *ssl = NULL;

  // link bio channel, SSL session, and server endpoint

  sprintf(name, "%s:%s", hostname, "https");
  BIO_get_ssl(bio, &ssl);                 // session 
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY); // robustness
  BIO_set_conn_hostname(bio, name);       // prepare to connect by setting the host name

  /* try to connect */
  if (BIO_do_connect(bio) <= 0)
  {
    cleanup(ctx, bio);
    report_and_exit("BIO_do_connect...");
  }

  // verify truststore, check cert
  if (!SSL_CTX_load_verify_locations(ctx,
                                     "/etc/ssl/certs/ca-certificates.crt",
                                     "/etc/ssl/certs/"))                   
    report_and_exit("SSL_CTX_load_verify_locations...");

  long verify_flag = SSL_get_verify_result(ssl);
  if (verify_flag != X509_V_OK)
    fprintf(stderr,
            "##### Certificate verification error (%i)\n",
            (int)verify_flag);

  // now fetch the webpage
  sprintf(request,
          "GET /%s HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n", // 1.0 is for getting one responce and end the connection
          page, hostname);                                             // 1.1 is for getting many responce without ending server 
  BIO_puts(bio, request);
  printf("Request Send to server \nWeb page loading to Result Folder\n");

  Buffer_node* Html_data = NULL;  // test

  while (1)
  {
    memset(response, '\0', sizeof(response));
    int n = BIO_read(bio, response, BuffSize);
    if (n <= 0)   // 0 is end-of-stream, < 0 is an error
      break;
    Html_data = Add_buffer(response,Html_data); // test
  }
  printf("Fetch data completed\n");
  cleanup(ctx, bio);
  return Html_data;
}

Buffer_node* fetch(char *hostname)
{
  init_ssl();
  char Domain_name[100];
  char Page_name[100];
  Break_Address(Domain_name, Page_name, hostname);
  fprintf(stderr, "Trying an HTTPS connection to %s%s...\n", Domain_name, Page_name);
  return secure_connect(Domain_name, Page_name);
}

// gcc demo.c -lssl -lcrypto
#include <stdio.h>
#include <stdlib.h>
#include <openssl/bio.h> /* BasicInput/Output streams */
#include <openssl/err.h> /* errors */
#include <openssl/ssl.h> /* core library */
#include <string.h>
#include "Socket.h"

#define BuffSize 1024

void report_and_exit(const char *msg)
{
  perror(msg);
  ERR_print_errors_fp(stderr);
  exit(-1);
}

void init_ssl()
{
  SSL_load_error_strings();
  SSL_library_init();
}

void cleanup(SSL_CTX *ctx, BIO *bio)
{
  SSL_CTX_free(ctx);
  BIO_free_all(bio);
}

void secure_connect(const char *hostname, char *page)
{
  char name[BuffSize];
  char request[BuffSize];
  char response[BuffSize];

  const SSL_METHOD *method = TLSv1_2_client_method();
  if (NULL == method)
    report_and_exit("TLSv1_2_client_method...");

  SSL_CTX *ctx = SSL_CTX_new(method);
  if (NULL == ctx)
    report_and_exit("SSL_CTX_new...");

  BIO *bio = BIO_new_ssl_connect(ctx);
  if (NULL == bio)
    report_and_exit("BIO_new_ssl_connect...");

  SSL *ssl = NULL;

  /* link bio channel, SSL session, and server endpoint */

  sprintf(name, "%s:%s", hostname, "https");
  BIO_get_ssl(bio, &ssl);                 /* session */
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY); /* robustness */
  BIO_set_conn_hostname(bio, name);       /* prepare to connect */

  /* try to connect */
  if (BIO_do_connect(bio) <= 0)
  {
    cleanup(ctx, bio);
    report_and_exit("BIO_do_connect...");
  }

  /* verify truststore, check cert */
  if (!SSL_CTX_load_verify_locations(ctx,
                                     "/etc/ssl/certs/ca-certificates.crt", /* truststore */
                                     "/etc/ssl/certs/"))                   /* more truststore */
    report_and_exit("SSL_CTX_load_verify_locations...");

  long verify_flag = SSL_get_verify_result(ssl);
  if (verify_flag != X509_V_OK)
    fprintf(stderr,
            "##### Certificate verification error (%i) but continuing...\n",
            (int)verify_flag);

  /* now fetch the homepage as sample data */
  sprintf(request,
          "GET /%s HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n",
          page, hostname);
  BIO_puts(bio, request);

  FILE *out_file = fopen("./Result/Html_Page.html", "w");
  while (1)
  {
    memset(response, '\0', sizeof(response));
    int n = BIO_read(bio, response, BuffSize);
    if (n <= 0)
      break; /* 0 is end-of-stream, < 0 is an error */
    puts(response);
    fprintf(out_file, "%s", response);
  }
  fclose(out_file);
  cleanup(ctx, bio);
}

void fetch(char *hostname)
{
  init_ssl();
  char Domain_name[100];
  char Page_name[100];
  Break_Address(Domain_name, Page_name, hostname);
  fprintf(stderr, "Trying an HTTPS connection to %s%s...\n", Domain_name, Page_name);
  secure_connect(Domain_name, Page_name);
}

// gcc demo.c -lssl -lcrypto
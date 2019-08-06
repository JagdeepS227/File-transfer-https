#include <stdio.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <curl/curl.h>
#include "ini/ini.h"

char CERTIFICATE[5000];
char PRIVATE_TRANSFER_KEY[5000];

static size_t writefunction(void *ptr, size_t size, size_t nmemb, void *stream);
static size_t write_to_file(void *ptr, size_t size, size_t nmemb, FILE *stream);
char* load_file_data(char *file);
static CURLcode sslctx_function(CURL *curl, void *sslctx, void *parm);
static void set_certificate_and_key(char *config_file);
char* get_save_file_location(char* save_location, char* file);
char* get_mac_address_linux();

int send_to_server(char *api, char *file, char *filename_tag, char *file_tag)
{
       

    CURL *curl;
    CURLcode res;
   

    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    struct curl_slist *headerlist = NULL;
    static const char buf[] = "Expect:";
    //char authorization_header[120] = "Authorization: ";
    
    //strcat(authorization_header, api_key);

    curl_global_init(CURL_GLOBAL_ALL);

    /* Fill in the file upload field 
    * if you need to change the type ofimage to send, modify this
    */
   puts("!!!!!!!!!!");    

    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, file_tag,
                 CURLFORM_FILE, file,
                 CURLFORM_END);

    /* Fill in the filename field */
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, filename_tag,
                 CURLFORM_COPYCONTENTS, file,
                 CURLFORM_END);

    /* Fill in the submit field too, even if this is rarely needed */
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, "submit",
                 CURLFORM_COPYCONTENTS, "client_s",
                 CURLFORM_END);
    fprintf(stdout, "Added form parts\n");

    curl = curl_easy_init();

    char output_filename[100]="server_data.txt";
    FILE *fp ;
    fp=fopen(output_filename,"wb");
if(fp!=NULL){
    printf("FILE OPEN!!!!!!!!!!!!!");
}
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_file);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunction);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, stderr);
    curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");

    /* initialize custom header list (stating that Expect: 100-continue is not
     wanted */
    headerlist = curl_slist_append(headerlist, buf);
    //headerlist = curl_slist_append(headerlist, authorization_header);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    if (curl)
    {
        fprintf(stdout, "****cURL initialized****\n");
        /* what URL that receives this POST */
        curl_easy_setopt(curl, CURLOPT_URL, api);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

            /* always cleanup */
            curl_easy_cleanup(curl);

            /* then cleanup the formpost chain */
            curl_formfree(formpost);
            /* free slist */
            curl_slist_free_all(headerlist);

            return -1;
        }

        /* always cleanup */
        curl_easy_cleanup(curl);

        /* then cleanup the formpost chain */
        curl_formfree(formpost);
        /* free slist */
        curl_slist_free_all(headerlist);
    }
fclose(fp);
    return 0;
}


int send_to_server_https(char *api, char *file, char *filename_tag, char *file_tag,char *config_file)
{
    puts("&&&&&&&&&&&&&&&");
    CURL *curl;
    CURLcode res;
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    struct curl_slist *headerlist = NULL;
    char save_location[100] = "/home/";
    static const char buf[] = "Expect:";
    char api_key;
    //char authorization_header[120] = "Authorization: ";
    //strcat(authorization_header, api_key);
    //fprintf(stdout, "%s\n", authorization_header);

    set_certificate_and_key(config_file);
puts("check!!!");
    curl_global_init(CURL_GLOBAL_ALL);
    /* Fill in the file upload field 
    * if you need to change the type ofimage to send, modify this
    */
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, file_tag,
                 CURLFORM_FILE, file,
                 CURLFORM_END);

    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, filename_tag,
                 CURLFORM_COPYCONTENTS, file,
                 CURLFORM_END);

    // Fill in the submit field too, even if this is rarely needed 
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, "submit",
                 CURLFORM_COPYCONTENTS, "client_s",
                 CURLFORM_END);
    fprintf(stdout, "Added form parts\n");

    curl = curl_easy_init();
    
     curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
     curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
     curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
     curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
     char output_filename[100]="update_folder.zip";
    FILE* save_file = fopen(output_filename, "wb");
    if(save_file == NULL){
        fprintf(stdout, "Error opening text file. Possibly a memory allocation issue.\nExiting...");
        return -1;
    }
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_file);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, save_file);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunction);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, stderr);
    curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");

    // initialize custom header list (stating that Expect: 100-continue is not wanted 
    headerlist = curl_slist_append(headerlist, buf);
    //headerlist = curl_slist_append(headerlist, authorization_header);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    if (curl)
    {
        fprintf(stdout, "****cURL initialized****\n");
        // what URL that receives this POST 
        curl_easy_setopt(curl, CURLOPT_URL, api);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);











#ifdef SKIP_PEER_VERIFICATION
        /*
            * If you want to connect to a site who isn't using a certificate that is
            * signed by one of the certs in the CA bundle you have, you can skip the
            * verification of the server's certificate. This makes the connection
            * A LOT LESS SECURE.
            *
            * If you have a CA cert for the server stored someplace else than in the
            * default bundle, then the CURLOPT_CAPATH option might come handy for
            * you.
            * Use if by defining SKIP_PEER_VERIFICATION before the #ifdef ie.e #define SKIP_PEER_VERIFICATION 1
            */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
        /*
            * If the site you're connecting to uses a different host name that what
            * they have mentioned in their server certificate's commonName (or
            * subjectAltName) fields, libcurl will refuse to connect. You can skip
            * this check, but this will make the connection less secure.
            * Use if by defining SKIP_HOSTNAME_VERIFICATION before the #ifdef ie.e #define SKIP_HOSTNAME_VERIFICATION 1
            */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
        // setiing self-signed certificate 
        curl_easy_setopt(curl, CURLOPT_CAINFO, CERTIFICATE);
        curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, *sslctx_function);
        // Perform the request, res will get the return code *
        
        res = curl_easy_perform(curl);
        puts("per");
        // Check for errors *
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

            // always cleanup *
            curl_easy_cleanup(curl);
            curl_formfree(formpost);
            curl_slist_free_all(headerlist);

            return -1;
        }

        // always cleanup *
        curl_easy_cleanup(curl);
        // then cleanup the formpost chain *
        curl_formfree(formpost);
        // free slist 
        curl_slist_free_all(headerlist);
        fclose(save_file);
    }

    return 0;
}


static size_t writefunction(void *ptr, size_t size, size_t nmemb, void *stream)
{
    fwrite(ptr, size, nmemb, stream);
    return (nmemb * size);
}

static size_t write_to_file(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

char* load_file_data(char *file)
{
    FILE *infile;
    char *buffer;
    long size;

    infile = fopen(file, "r");
    if (infile == NULL)
    {
        fprintf(stdout, "Error loading file:- %s . Please check if the file exists in the given location\n", file);
        return NULL;
    }

    fseek(infile, 0L, SEEK_END);
    size = ftell(infile);
    fseek(infile, 0L, SEEK_SET);
    buffer = (char *)calloc(size, sizeof(char));
    if (buffer == NULL)
    {
        fprintf(stdout, "Error reading file:- %s", file);
        return NULL;
    }

    fread(buffer, sizeof(char), size, infile);
    fclose(infile);

    return buffer;
}

static CURLcode sslctx_function(CURL *curl, void *sslctx, void *parm)
{
    X509 *cert = NULL;
    BIO *bio = NULL;
    BIO *kbio = NULL;
    RSA *rsa = NULL;
    int ret;

    char *mypem = load_file_data(CERTIFICATE);
    char *mykey = load_file_data(PRIVATE_TRANSFER_KEY);

    (void)curl; /* avoid warnings */
    (void)parm; /* avoid warnings */

    /* get a BIO */
    bio = BIO_new_mem_buf((char *)mypem, -1);

    if (bio == NULL)
    {
        fprintf(stdout, "BIO_new_mem_buf failed\n");
    }

    /* use it to read the PEM formatted certificate from memory into an X509
   * structure that SSL can use
   */
    cert = PEM_read_bio_X509(bio, NULL, 0, NULL);
    if (cert == NULL)
    {
        fprintf(stdout, "PEM_read_bio_X509 failed...\n");
    }

    /*tell SSL to use the X509 certificate*/
    ret = SSL_CTX_use_certificate((SSL_CTX *)sslctx, cert);
    if (ret != 1)
    {
        fprintf(stdout, "Use certificate failed\n");
    }

    /*create a bio for the RSA key*/
    kbio = BIO_new_mem_buf((char *)mykey, -1);
    if (kbio == NULL)
    {
        fprintf(stdout, "BIO_new_mem_buf failed\n");
    }

    /*read the key bio into an RSA object*/
    rsa = PEM_read_bio_RSAPrivateKey(kbio, NULL, 0, NULL);
    if (rsa == NULL)
    {
        fprintf(stdout, "Failed to create key bio\n");
    }

    /*tell SSL to use the RSA key from memory*/
    ret = SSL_CTX_use_RSAPrivateKey((SSL_CTX *)sslctx, rsa);
    if (ret != 1)
    {
        fprintf(stdout, "Use Key failed\n");
    }

    /* free resources that have been allocated by openssl functions */
    if (bio)
        BIO_free(bio);

    if (kbio)
        BIO_free(kbio);

    if (rsa)
        RSA_free(rsa);

    if (cert)
        X509_free(cert);

    /* free resources allocated for certificate and encryption key */
    if (mypem)
        free(mypem);

    if (mykey)
        free(mykey);

    /* all set to go */
    return CURLE_OK;
}

static void set_certificate_and_key(char *config_file)
{
    ini_t *config = ini_load(config_file);

    strcpy(CERTIFICATE, ini_get(config, "Web_Server", "CERTIFICATE_FILE"));
puts(CERTIFICATE);
    strcpy(PRIVATE_TRANSFER_KEY, ini_get(config, "User", "ENCRYPTION_KEY_FILE"));
    puts(PRIVATE_TRANSFER_KEY);
    
}

char* get_mac_address_linux(){
    FILE *file;
    char *mac = calloc(20, sizeof(char));

    file = fopen("/sys/class/net/wlan0/address", "r");
    if (file == NULL)
    {
        fprintf(stdout, "Error loading mac address file:- /sys/class/net/wlan0/address . Corrupt device.\n");
        return NULL;
    }

    fgets(mac, 17, file);
    if(mac == NULL){
        fprintf(stdout, "No MAC address is set for hardware of device. MAC: %s", mac);
        return NULL;
    }
    return mac;
}

char* get_save_file_location(char* save_location, char* file){
    char *save_file_location = calloc(200, sizeof(char));
    char filename[50];
    strcpy(save_file_location, save_location);

    char *pp, *p;
    // Eg *image = /home/fa/images/eng_9273920102_001.jpg
    pp = p = strtok(file, "/");
    while(p != NULL){
        p = strtok(NULL, "/");
        if(p != NULL){
            pp = p;
        }
    }

    p = strtok(pp, ".");
    strcpy(filename, p);
    strcat(filename, ".txt");
    strcat(save_file_location, filename);

    return save_file_location;
}

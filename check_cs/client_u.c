
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini/ini.h"
#include "send.h"
#include <mysql/mysql.h>
#include <dirent.h> 

int main(){



                            char success_sync[10];
                        FILE *fp3;
                        char* filename3 = "acknowledge";
                            
     //fputs("\n",fp1);


    char file[100];
    char api_key[200];
    char protocol[200];
    char url[200];
    char port[6];
    char api[100];
    char filename_tag[50];
    char file_tag[50];
    char mac_tag[50];
    char contact_tag[50];
    char file_transfer_api[400];
    char config_file[400];
    char user_info_file[200];

    //system("zip -r file_to_send.zip file_to_send");


    // checking if file is given
    
        
        strcpy(file, "client_version.txt");
        strcpy(config_file, "client.config");
        strcpy(user_info_file, "user_out.txt");
        fprintf(stdout, "File passed = %s\nConfig file = %s\n", file, config_file);
    
 
    ini_t *config = ini_load(config_file);
    strcpy(protocol, ini_get(config, "Web_Server", "PROTOCOL"));
    strcpy(url, ini_get(config, "Web_Server", "URL"));
    strcpy(port, ini_get(config, "Web_Server", "PORT"));
    strcpy(api, ini_get(config, "apis", "UPDATE"));
    strcpy(filename_tag, ini_get(config, "form_fields", "FILE_NAME_TAG"));
    strcpy(file_tag, ini_get(config, "form_fields", "FILE_TAG"));
    
    strcpy(file_transfer_api, protocol);

    strcat(file_transfer_api, "://");
    strcat(file_transfer_api, url);
    strcat(file_transfer_api, ":");
    strcat(file_transfer_api, port);
    strcat(file_transfer_api, "/");
    strcat(file_transfer_api, api);
puts("*******");    

    

    int is_send_successful; 
    if( strcmp(protocol, "http") == 0 ){
        is_send_successful = send_to_server(file_transfer_api, file, filename_tag, file_tag);
    }

    else if( strcmp(protocol, "https") == 0 ) {   puts("!!!#####");    
	puts(file_transfer_api);
            is_send_successful = send_to_server_https(file_transfer_api, file, filename_tag, file_tag,config_file);

    }
puts("!!!!***");    

    if(is_send_successful == 0){
        fprintf(stdout, "\n\nINFO: FILE sent successfully.\n\n");

        fp3 = fopen(filename3, "r+");       
        fgets(success_sync,10, fp3);
        if(success_sync=="ok"){
            //system("rm  /home/app/Downloads/update_client/client_server.txt "); 
            //system("rm -r /home/app/Downloads/https_transfer_work/check_cs/file_to_send "); 
        }
        fclose(fp3);
    }
    else{
        fprintf(stdout, "\n\nWARNING: FILE sending failed. Exiting...\n\n");
    }
puts("*******");
    return 0;
}

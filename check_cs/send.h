

#ifndef CLIENT_SEND_H
#define CLIENT_SEND_H

#define CLIENT_SEND_VERSION 0.1.1

// TODO: Provide a definition of the function and the return value
int send_to_server(char *api, char* file, char *filename_tag, char *file_tag);
int send_to_server_https(char *api, char *file, char *filename_tag, char *file_tag, char *config_file);

#endif

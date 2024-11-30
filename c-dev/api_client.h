#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <curl/curl.h>
#include <cjson/cJSON.h>

typedef struct {
    CURL* curl;
} ApiClient;

ApiClient* init_api_client(void);
cJSON* get_data(ApiClient* client, char* url, char* header, char* post_data);
void api_clean_up(ApiClient* client);

#endif
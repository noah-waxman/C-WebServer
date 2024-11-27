#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <cjson/cJSON.h>

#define BUFFER_SIZE 32768  // 32KB buffer
#define UNRESTRICT_URL "https://api.real-debrid.com/rest/1.0/unrestrict/link?auth_token=" API_KEY
#define API_KEY "IHYRFIB7VTHNJ2WGLUFCXO4YUI2NGNPM2NW4GVUWQ5F7OXK2KLAA"
#define BASE_URL "https://api.real-debrid.com/rest/1.0/downloads?auth_token=" API_KEY
#define TRANSCODE_URL "https://api.real-debrid.com/rest/1.0/streaming/transcode/7MB2E5MJA2P2M?auth_token=" API_KEY

typedef struct {
    char id[12];
    char filename[255];
    char link[255];
    double filesize;
} Media;

typedef struct {
    unsigned char *buffer;
    size_t len;
    size_t buflen;
} get_request;

#define CHUNK_SIZE 2048

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    get_request *req = (get_request *) userdata;

    printf("receive chunk of %zu bytes\n", realsize);

    while (req->buflen < req->len + realsize + 1) {
        req->buffer = realloc(req->buffer, req->buflen + CHUNK_SIZE);
        req->buflen += CHUNK_SIZE;
    }
    memcpy(&req->buffer[req->len], ptr, realsize);
    req->len += realsize;
    req->buffer[req->len] = 0;

    return realsize;
}


void get_downloads() {
    CURL *curl = curl_easy_init();
    get_request req = {.buffer = NULL, .len = 0, .buflen = 0};

    // Initialize buffer first
    req.buffer = malloc(CHUNK_SIZE);
    req.buflen = CHUNK_SIZE;

    if (curl) {
        // Set options before performing the request
        curl_easy_setopt(curl, CURLOPT_URL, BASE_URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&req);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        cJSON *json = cJSON_Parse((char*)req.buffer);
        printf("%s", req.buffer);

        int array_size = cJSON_GetArraySize(json);
        printf("Array has %d\n items", array_size);

        for (int i = 0; i < array_size; i++) {
            Media film;
            cJSON *item = cJSON_GetArrayItem(json, i);
            char *id = cJSON_GetObjectItem(item, "id")->valuestring;
            char *filename = cJSON_GetObjectItem(item, "filename")->valuestring;
            char *link = cJSON_GetObjectItem(item, "link")->valuestring;
            strncpy(film.id, id, sizeof(film.id) - 1);
            strncpy(film.filename, filename, sizeof(film.filename) - 1);
            strncpy(film.link, link, sizeof(film.link) - 1);
            film.id[sizeof(film.id) - 1] = '\0'; 
            film.filename[sizeof(film.filename) - 1] = '\0'; 
            film.link[sizeof(film.link) - 1] = '\0'; 
            printf("\nItem: %s", film.link);
            
        }

        // Cleanup
        free(req.buffer);
        curl_easy_cleanup(curl);
    }
    return;
}


int main(void) {
    get_downloads();
    return 0;
}
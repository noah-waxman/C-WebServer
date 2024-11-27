#include "api_client.h"
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

cJSON* get_data(ApiClient* client, char* url) {
    struct MemoryStruct chunk;
    CURLcode res;
    
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_easy_setopt(client->curl, CURLOPT_URL, url);
    curl_easy_setopt(client->curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(client->curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(client->curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    res = curl_easy_perform(client->curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    } else {
        printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
    }
    
    cJSON *json = cJSON_Parse(chunk.memory);
    free(chunk.memory);
    curl_easy_cleanup(client->curl);
    return json;
}

ApiClient* init_api_client(void) {
    ApiClient* client = malloc(sizeof(ApiClient));
    client->curl = curl_easy_init();

    if (!client->curl) {
        fprintf(stderr, "Failed to initialize curl.\n");
    }

    return client;
}
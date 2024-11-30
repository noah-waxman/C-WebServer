#include "api_client.h"
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

/* Structure to hold memory for CURL response data */
struct MemoryStruct {
    char *memory;   /* Pointer to the response data buffer */
    size_t size;    /* Current size of the buffer */
};

/* Callback function for CURL to write received data */
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    /* Reallocate memory to accommodate new data */
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        /* Handle memory allocation failure */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    /* Copy new data to the buffer */
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;  /* Null terminate the string */

    return realsize;
}

/* Function to fetch data from a URL and return it as JSON */
cJSON* get_data(ApiClient* client, char* url, char* header, char* post_data) {
    struct MemoryStruct chunk;
    CURLcode res;
    struct curl_slist *headers = NULL;
    
    /* Initialize memory chunk */
    chunk.memory = malloc(1);
    chunk.size = 0;

    /* Configure CURL options */
    curl_easy_setopt(client->curl, CURLOPT_URL, url);
    curl_easy_setopt(client->curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(client->curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(client->curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* Check if header parameter was given */
    if (header != NULL) {
        headers = curl_slist_append(headers, header);
        curl_easy_setopt(client->curl, CURLOPT_HTTPHEADER, headers);
    }

    /* Check if post_data parameter was given; if so, we POST */
    if (post_data != NULL) {
        curl_easy_setopt(client->curl, CURLOPT_POSTFIELDS, post_data);
    }

    /* Perform the HTTP request */
    res = curl_easy_perform(client->curl);

    /* Check for errors */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    } else {
        printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
    }
    
    /* Parse response as JSON and clean up */
    cJSON *json = cJSON_Parse(chunk.memory);
    free(chunk.memory);
    return json;
}

/* Function to initialize the API client */
ApiClient* init_api_client(void) {
    /* Allocate memory for the client structure */
    ApiClient* client = malloc(sizeof(ApiClient));
    client->curl = curl_easy_init();

    /* Check if CURL initialization was successful */
    if (!client->curl) {
        fprintf(stderr, "Failed to initialize curl.\n");
    }

    return client;
}

void api_clean_up(ApiClient* client) {
    curl_easy_cleanup(client->curl);
    curl_global_cleanup();
}

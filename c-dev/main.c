#include <stdio.h>
#include "api_client.h"
#include "media_collection.h"
#include <cjson/cJSON.h>
#include <string.h>
#define API_KEY "IHYRFIB7VTHNJ2WGLUFCXO4YUI2NGNPM2NW4GVUWQ5F7OXK2KLAA"
#define DOWNLOADS_URL "https://api.real-debrid.com/rest/1.0/downloads?auth_token=" API_KEY

// Function to parse and process Real-Debrid downloads from JSON response
void get_rd_downloads(MediaCollection* collection, cJSON *json) {
    // Get the number of items in the JSON array
    int array_size = cJSON_GetArraySize(json);

    // Iterate through each item in the JSON array
    for (int i = 0; i < array_size; i++) {
        Media film;
        
        // Get the current array item
        cJSON *item = cJSON_GetArrayItem(json, i);
        
        // Extract values from JSON object
        char *id = cJSON_GetObjectItem(item, "id")->valuestring;
        char *filename = cJSON_GetObjectItem(item, "filename")->valuestring;
        char *link = cJSON_GetObjectItem(item, "link")->valuestring;

        // Copy values to Media structure with bounds checking
        strncpy(film.id, id, sizeof(film.id) - 1);
        strncpy(film.filename, filename, sizeof(film.filename) - 1);
        strncpy(film.link, link, sizeof(film.link) - 1);
        film.filesize = 0.0;

        // Ensure null termination of strings
        film.id[sizeof(film.id) - 1] = '\0'; 
        film.filename[sizeof(film.filename) - 1] = '\0'; 
        film.link[sizeof(film.link) - 1] = '\0'; 

        add_media(collection, film);
    }
}

int main(void) {
    ApiClient* client = init_api_client();
    cJSON *json = get_data(client, DOWNLOADS_URL);
    MediaCollection* collection = create_media_collection();
    get_rd_downloads(collection, json);
    print_media(collection);
    printf("%zu\n", collection->count);
    api_clean_up(client);
    return 0;
}
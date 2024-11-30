#include <stdio.h>
#include "api_client.h"
#include "media_collection.h"
#include <cjson/cJSON.h>
#include <string.h>
#include <stdlib.h>
#define API_KEY "IHYRFIB7VTHNJ2WGLUFCXO4YUI2NGNPM2NW4GVUWQ5F7OXK2KLAA"
#define DOWNLOADS_URL "https://api.real-debrid.com/rest/1.0/downloads?auth_token=" API_KEY
#define UNRESTRICT_URL "https://api.real-debrid.com/rest/1.0/unrestrict/link?auth_token=" API_KEY
#define TRANSCODE_URL "https://api.real-debrid.com/rest/1.0/streaming/transcode/?auth_token=" API_KEY
#define AUTH "?auth_token=" API_KEY

// Function: get_rd_downloads
// ---------------------------
// Parses a JSON response containing Real-Debrid downloads and populates a MediaCollection.
//
// Parameters:
// - collection: A pointer to a MediaCollection to store parsed media.
// - json: A cJSON object representing the parsed JSON response.
//
// Usage Example:
// MediaCollection* collection = create_media_collection();
// cJSON *json = get_data(client, DOWNLOADS_URL, NULL, NULL);
// get_rd_downloads(collection, json);
// print_media(collection);
//
// After usage, ensure to clean up memory for collection and JSON data using cJSON_Delete.
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

        // For now, I don't need the filesize. I set it to zero.
        film.filesize = 0.0;

        // Ensure null termination of strings
        film.id[sizeof(film.id) - 1] = '\0'; 
        film.filename[sizeof(film.filename) - 1] = '\0'; 
        film.link[sizeof(film.link) - 1] = '\0'; 

        add_media(collection, film);
    }
}

// Function: get_stream
// --------------------
// Retrieves stream information for a given media item by its ID using Real-Debrid's API.
//
// Parameters:
// - client: A pointer to an initialized ApiClient used for making API requests.
// - id: A string representing the ID of the media item to retrieve stream data.
//
// Usage Example:
// ApiClient* client = init_api_client();
// get_stream(client, "media_id_here");
// api_clean_up(client);
//
// Ensure the ApiClient is initialized and cleaned up before and after usage.
void get_stream(ApiClient* client, char* id) {
    // Create buffer to create api link
    char buffer[1024] = {0};
    snprintf(buffer, sizeof(buffer), "https://api.real-debrid.com/rest/1.0/streaming/transcode/%s%s", id, AUTH);

    // Print Link
    // printf("\n%s\n", buffer);

    // GET request to /streaming/transcode/{id}
    cJSON* json = get_data(client, buffer, NULL, NULL);

    // Print JSON
    // char* string = cJSON_Print(json);
    // printf("\njson response: %s\n", string);

    cJSON *mp4_item = cJSON_GetObjectItem(json, "liveMP4");
    if (cJSON_IsObject(mp4_item)) {
        cJSON *full_item = cJSON_GetObjectItem(mp4_item, "full");
        if (cJSON_IsString(full_item) && (full_item->valuestring != NULL)) {
            printf("\nmp4 Full URL: %s\n", full_item->valuestring);
        } else {
            printf("\nKey 'full' not found or is not a string inside 'mp4'.\n");
        }
    } else {
        printf("\nKey 'mp4' not found or is not an object.\n");
    }
    cJSON_Delete(json);
}

int main(void) {
    ApiClient* client = init_api_client();
    cJSON *json = get_data(client, DOWNLOADS_URL, NULL, NULL);
    MediaCollection* collection = create_media_collection();

    get_rd_downloads(collection, json);

    cJSON_Delete(json);

    print_media(collection);
    
    Media item = collection->items[0];
    printf("\n\nNIGGER %s\n", item.id);

    get_stream(client, item.id);

    api_clean_up(client);
    return 0;
}
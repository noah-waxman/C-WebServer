#include "media_collection.h"
#include <stdlib.h>
#include <stdio.h>

#define INITIAL_CAPACITY 10

MediaCollection* create_media_collection() {
    /* Initialize the collection */
    MediaCollection* collection = malloc(sizeof(MediaCollection));
    if (collection == NULL) {
        return NULL;
    }

    /* Create the media items array */
    collection->items = malloc(INITIAL_CAPACITY * sizeof(Media));
    if (collection->items == NULL) {
        free(collection);
        return NULL;
    }

    collection->count = 0;
    collection->capacity = INITIAL_CAPACITY;
    return collection;
}

int add_media(MediaCollection* collection, Media media) {
    // Check if the collection is at full capacity
    if (collection->count == collection->capacity) {
        // Double the capacity when full
        int new_capacity = collection->capacity * 2;
        // Reallocate items array
        Media* new_items = realloc(collection->items, new_capacity * sizeof(Media));
        if (new_items != NULL) {
            collection->items = new_items;
            collection->capacity = new_capacity;
        }
    }

    // Add item
    if (collection->count < collection->capacity) {
        collection->items[collection->count] = media;
        collection->count++;
        return 1;
    }

    return -1;
}

void print_media(MediaCollection* collection) {
    // int array_size = sizeof(collection) / sizeof(collection[0]);
    for (size_t i = 0; i < collection->count; i++) {
        puts("-----------------");
        printf("%s\n", collection->items[i].id);
        printf("%s\n", collection->items[i].filename);
        printf("%s\n", collection->items[i].link);
    }
}
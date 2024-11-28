#ifndef MEDIA_COLLECTION_H
#define MEDIA_COLLECTION_H
#include "media.h"
#include <stddef.h>

// Structure to store Media
typedef struct {
    Media* items;       // Dynamic array of Media
    size_t count;       // Current number of items
    size_t capacity;    
} MediaCollection;

MediaCollection* create_media_collection(void);
int add_media(MediaCollection* collection, Media media);
void print_media(MediaCollection* collection);

#endif
#ifndef MEDIA_H
#define MEDIA_H

// Structure to store media download information
typedef struct {
    char id[50];        // Unique identifier for the download
    char filename[255]; // Name of the downloaded file
    char link[255];     // Download URL
    char download[255]; // Un-Restricted Download Link
    double filesize;    // Size of the file in bytes
} Media;

#endif
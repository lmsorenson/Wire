#include "../include/wire.h"

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_SIZE 5
#define SUCCESS 0
#define DIRECTORY_NOT_FOUND 1

int listDevices(char * search_directory, char *dst[ARRAY_SIZE])
{
    printf("\nSearching the /dev/ directory for usb modems.\n");

    struct dirent * dir_entry;
    DIR * active_directory = opendir(search_directory);

    if(active_directory == NULL)
    {
        printf("Could not find %s directory.", search_directory);
        free(dir_entry);
        dir_entry = NULL;
        closedir(active_directory);
        return DIRECTORY_NOT_FOUND;
    }

    for(int i=0; i < ARRAY_SIZE; i++)
        dst[i] = (char*)malloc(256 * sizeof(char));

    int count_modems = 0;
    while ((dir_entry = readdir(active_directory)) != NULL)
    {
        if(strncmp(dir_entry->d_name, "cu.usbmodem", 11)==0)
        {
            printf("Found modem: %s\n\n", dir_entry->d_name);
            printf("writing output-string[%d]", count_modems);
            strncpy(dst[count_modems], search_directory, sizeof(search_directory));
            strncat(dst[count_modems], dir_entry->d_name, sizeof(dir_entry->d_name));

            //only count matching devices that will be added to the list
            count_modems++;
        }
    }

    free(dir_entry);
    dir_entry = NULL;
    closedir(active_directory);
    return SUCCESS;
}

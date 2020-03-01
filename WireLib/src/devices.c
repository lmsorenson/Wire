#include "../include/wire.h"

#include <stdio.h>
#include <dirent.h>
#include <string.h>

int listDevices(char * search_directory, char * dst)
{
    printf("\nSearching the /dev/ directory for usb modems.\n");

    struct dirent * dir_entry;
    DIR * active_directory = opendir(search_directory);

    if(active_directory==NULL)
    {
        printf("Could not find /dev/ directory.");
        return 1;
    }

    while ((dir_entry = readdir(active_directory)) != NULL) 
    {
        if(strncmp(dir_entry->d_name, "cu.usbmodem", 11)==0)
        {
            printf("Found modem: %s\n\n", dir_entry->d_name); 
            strcpy(dst, search_directory);
            strcat(dst, dir_entry->d_name);
            return 0;
        }
    }
  
    closedir(active_directory); 

    return 0;
}
#include "../include/wire.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int GetDescriptorValue(char str[], char ch)
{

    char buffer[1024];
    strcpy(buffer, str);

    char* pch = strtok(buffer, ".");

    while(pch!=NULL)
    {
        if( pch[0] == ch )
        {
            char value_buffer[strlen(pch)];
            memcpy(value_buffer, &pch[1], strlen(pch));

            return atoi(value_buffer);
        }

        pch = strtok(NULL, ".");
    }

    return -1;
}
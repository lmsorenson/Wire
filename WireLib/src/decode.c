#include "../include/wire.h"

#include <stdio.h>

int GetDescriptor(char str[])
{
    printf("%s\n", str);


    char* pch = strtok(str, ".");



    while(pch!=NULL)
    {


        // printf("%s\n", pch);

        // printf("buffer: %s, character: %s\n", local_buffer, character);

        // if( strcmp(local_buffer, character)==0 )
        // {
        //     char value_buffer[strlen(pch)];
        //     memcpy(value_buffer, &pch[1], strlen(pch));

            

        //     return atoi(value_buffer);
        // }

        pch = strtok(NULL, ".");

    }

    return -1;
}
#include <wire.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int fileDescriptor;

    char* dev_name[5];
    char* err = (char*)malloc(1024 * sizeof(char));
    listDevices("/dev/", dev_name);

    //open the serial port
    printf("Attempting to open port: %s", dev_name[0]);
    fileDescriptor = openSerialPort(dev_name[0], err);

    if(fileDescriptor != -1)
    {
        char* data = (char*)malloc(1024 * sizeof(char));
    
        writeSerialPort(fileDescriptor, "~Status", err);

        if(readSerialPort(fileDescriptor, data, err)!=0)
        {
            free(err);
            free(data);
            return 1;
        }
        else
        {
            //print data from the serial port.
            printf("\n\nSerial Port Data-%s\n", data);
            printf("B: %i\n", GetDescriptorValue(data, 'B'));
            printf("T: %i\n", GetDescriptorValue(data, 'T'));
            printf("S: %i\n", GetDescriptorValue(data, 'S'));

        }

        free(data);

        //close the serial port
        closeSerialPort(fileDescriptor);
    }


    for(int i=0; i<5; i++)
    {
        free(dev_name[i]);
    } 
    

    free (err);
    return 0;
}
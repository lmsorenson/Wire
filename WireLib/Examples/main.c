#include <wire.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int fileDescriptor;

    char* dev_name[5];

    listDevices("/dev/", dev_name);

    //open the serial port
    printf("Attempting to open port: %s", dev_name[0]);
    fileDescriptor=openSerialPort(dev_name[0]);

    if(fileDescriptor != -1)
    {

        char* data = (char*)malloc(1024 * sizeof(char));
    
        writeSerialPort(fileDescriptor, "~Status");

        if(readSerialPort(fileDescriptor, data)!=0)
        {
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
    

    return 0;
}
#include <wire.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int fileDescriptor;

    char* dev_name = (char*)malloc(1024 * sizeof(char));

    listDevices("/dev/", dev_name);

    //open the serial port
    fileDescriptor=openSerialPort(dev_name);

    if(fileDescriptor != -1)
    {

        char* data = (char*)malloc(1024 * sizeof(char));
    
        if(readSerialPort(fileDescriptor, data)!=0)
        {
            return 1;
        }
        else
        {
            //print data from the serial port.
            printf("\n\nSerial Port Data\n");
            printf("B: %i\n", GetDescriptorValue(data, 'B'));
            printf("T: %i\n", GetDescriptorValue(data, 'T'));
            printf("S: %i\n", GetDescriptorValue(data, 'S'));

        }
        

        free(data);

        //close the serial port
        closeSerialPort(fileDescriptor);

    }

    free(dev_name);

    return 0;
}
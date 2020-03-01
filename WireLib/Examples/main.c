#include <wire.h>
#include <stdio.h>

int main()
{
    int fileDescriptor;

    //open the serial port
    fileDescriptor=openSerialPort("/dev/cu.usbmodem14601");

    if(fileDescriptor != -1)
    {

        char* data;
    
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
        

        //close the serial port
        closeSerialPort(fileDescriptor);

    }

    return 0;
}
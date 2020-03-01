#include <wire.h>

int main()
{
    printf("hello world! \n");

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
            printf("%s", data);
            printf("\n");

            GetDescriptor(data);
        }
        

        //close the serial port
        closeSerialPort(fileDescriptor);

    }

    return 0;
}
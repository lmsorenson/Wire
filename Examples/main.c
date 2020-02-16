#include <wire.h>

void main()
{
    printf("hello world! \n");

    int fileDescriptor;

    //open the serial port
    fileDescriptor=openSerialPort("/dev/cu.usbmodem14601");

    //print data from the serial port.
    char* data = readSerialPort(fileDescriptor);

    //print data from the serial port.
    printf("\n\nSerial Port Data\n");
    printf(data);
    printf("\n");

    //close the serial port
    closeSerialPort(fileDescriptor);
}
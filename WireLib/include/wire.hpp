#ifndef WIRE_CPP
#define WIRE_CPP


extern "C"
{
    int openSerialPort(const char *bsdPath);
    char *logString(const char * str);
    char *logData(const char * str);
    int initializeModem(int fileDescriptor);
    void writeSerialPort(int fileDescriptor, char * InString);
    char *readSerialPort(int fileDescriptor);
    int closeSerialPort(int fileDescriptor);
}


#endif
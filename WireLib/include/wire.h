#ifndef WIRE
#define WIRE



int openSerialPort(const char *bsdPath);
int initializeModem(int fileDescriptor);
int closeSerialPort(int fileDescriptor);

int writeSerialPort(int fileDescriptor, char * InString, char * err);
int readSerialPort(int fileDescriptor, char * dst, char * err);

int GetDescriptorValue(char str[], char ch);
int listDevices(char * search_directory, char *dst[5]);


#endif
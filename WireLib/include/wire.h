#ifndef WIRE
#define WIRE



int openSerialPort(const char *bsdPath);
int initializeModem(int fileDescriptor);
int closeSerialPort(int fileDescriptor);

void writeSerialPort(int fileDescriptor, char * InString);
int readSerialPort(int fileDescriptor, char * dst);

int GetDescriptorValue(char str[], char ch);
int listDevices(char * search_directory, char *dst[5]);


#endif
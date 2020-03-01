#ifndef WIRE
#define WIRE



int openSerialPort(const char *bsdPath);
char *logString(const char * str);
char *logData(const char * str);
int initializeModem(int fileDescriptor);
void writeSerialPort(int fileDescriptor, char * InString);
int readSerialPort(int fileDescriptor, char * dst);
int closeSerialPort(int fileDescriptor);

int GetDescriptorValue(char str[], char ch);

int listDevices(char * search_directory, char * dst);


#endif
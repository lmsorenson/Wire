// Fill out your copyright notice in the Description page of Project Settings.

#include "../include/wire.h"

#include <sys/ioctl.h>
#include <termios.h>

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <paths.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/serial/ioss.h>
#include <IOKit/IOBSD.h>



static struct termios gOriginalTTYAttrs;

#define kATCommandString    "AT\r\n"
#define kENDCommandString   "END\r\n"


#ifdef LOCAL_ECHO
#define kOKResponseString   "AT\r\r\nOK\r\n"
#else
#define kOKResponseString   "\r\nOK\r\n"
#endif

const int kNumRetries = 3;

// Given the path to a serial device, open the device and configure it.
// Return the file descriptor associated with the device.
int openSerialPort(const char *bsdPath)
{
    int fileDescriptor = -1;    //File Descriptor
    int handshake;              //Handshake
    struct termios  options;    //Options
    
    speed_t speed;              //Baud Rate
    unsigned long mics;         //Read Latency
    
    // Open the serial port read/write, with no controlling terminal, and don't wait for a connection.
    // The O_NONBLOCK flag also causes subsequent I/O on the device to be non-blocking.
    // See open(2) <x-man-page://2/open> for details.
    
    fileDescriptor = open(bsdPath, O_RDWR | O_NOCTTY | O_NONBLOCK);
    
    if (fileDescriptor == -1) {
       printf("Error opening serial port %s - %s(%d).\n", bsdPath, strerror(errno), errno);
        goto error;
    }
    
    
    
    if (ioctl(fileDescriptor, TIOCEXCL) == -1) {
        printf("Error setting TIOCEXCL on %s - %s(%d).\n",
              bsdPath, strerror(errno), errno);
        goto error;
    }
    
    
    
    // Now that the device is open, clear the O_NONBLOCK flag so subsequent I/O will block.
    // See fcntl(2) <x-man-page//2/fcntl> for details.
    
    if (fcntl(fileDescriptor, F_SETFL, 0) == -1) {
        printf("Error clearing O_NONBLOCK %s - %s(%d).\n",
              bsdPath, strerror(errno), errno);
        goto error;
    }
    
    // Get the current options and save them so we can restore the default settings later.
    if (tcgetattr(fileDescriptor, &gOriginalTTYAttrs) == -1) {
        printf("Error getting tty attributes %s - %s(%d).\n",
               bsdPath, strerror(errno), errno);
        goto error;
    }
    
    // The serial port attributes such as timeouts and baud rate are set by modifying the termios
    // structure and then calling tcsetattr() to cause the changes to take effect. Note that the
    // changes will not become effective without the tcsetattr() call.
    // See tcsetattr(4) <x-man-page://4/tcsetattr> for details.
    
    options = gOriginalTTYAttrs;
    
    // Print the current input and output baud rates.
    // See tcsetattr(4) <x-man-page://4/tcsetattr> for details.
    
    printf("Current input baud rate is %d\n", (int) cfgetispeed(&options) );
    printf("Current output baud rate is %d\n", (int) cfgetospeed(&options) );
    
    // Set raw input (non-canonical) mode, with reads blocking until either a single character
    // has been received or a one second timeout expires.
    // See tcsetattr(4) <x-man-page://4/tcsetattr> and termios(4) <x-man-page://4/termios> for details.
    
    cfmakeraw(&options);
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 10;
    
    cfsetspeed(&options, B9600);       // Set 19200 baud
    //cfsetspeed(&options, B9600);       // Set 19200 baud
    options.c_cflag |= (CS7        |    // Use 7 bit words
                        PARENB     |    // Parity enable (even parity if PARODD not also set)
                        CCTS_OFLOW |    // CTS flow control of output
                        CRTS_IFLOW);    // RTS flow control of input
    
    // The IOSSIOSPEED ioctl can be used to set arbitrary baud rates
    // other than those specified by POSIX. The driver for the underlying serial hardware
    // ultimately determines which baud rates can be used. This ioctl sets both the input
    // and output speed.
    
    speed = 14400; // Set 14400 baud
    if (ioctl(fileDescriptor, IOSSIOSPEED, &speed) == -1) {
        printf("Error calling ioctl(..., IOSSIOSPEED, ...) %s - %s(%d).\n",
               bsdPath, strerror(errno), errno);
    }
    
    // Print the new input and output baud rates. Note that the IOSSIOSPEED ioctl interacts with the serial driver
    // directly bypassing the termios struct. This means that the following two calls will not be able to read
    // the current baud rate if the IOSSIOSPEED ioctl was used but will instead return the speed set by the last call
    // to cfsetspeed.
    
    printf("Input baud rate changed to %d\n", (int) cfgetispeed(&options) );
    printf("Output baud rate changed to %d\n", (int) cfgetospeed(&options) );
    
    // Cause the new options to take effect immediately.
    if (tcsetattr(fileDescriptor, TCSANOW, &options) == -1) {
        printf("Error setting tty attributes %s - %s(%d).\n",
              bsdPath, strerror(errno), errno);
        goto error;
    }
    
    
    // To set the modem handshake lines, use the following ioctls.
    // See tty(4) <x-man-page//4/tty> and ioctl(2) <x-man-page//2/ioctl> for details.
    
     //Handshake lines
     
    // Assert Data Terminal Ready (DTR)
    if (ioctl(fileDescriptor, TIOCSDTR) == -1) {
        printf("Error asserting DTR %s - %s(%d).\n",
              bsdPath, strerror(errno), errno);
    }
    
    // Clear Data Terminal Ready (DTR)
    if (ioctl(fileDescriptor, TIOCCDTR) == -1) {
        printf("Error clearing DTR %s - %s(%d).\n",
              bsdPath, strerror(errno), errno);
    }
               
    // Set the modem lines depending on the bits set in handshake
    handshake = TIOCM_DTR | TIOCM_RTS | TIOCM_CTS | TIOCM_DSR;
    if (ioctl(fileDescriptor, TIOCMSET, &handshake) == -1) {
        printf("Error setting handshake lines %s - %s(%d).\n",
               bsdPath, strerror(errno), errno);
    }

    // To read the state of the modem lines, use the following ioctl.
    // See tty(4) <x-man-page//4/tty> and ioctl(2) <x-man-page//2/ioctl> for details.
    
    // Store the state of the modem lines in handshake
    if (ioctl(fileDescriptor, TIOCMGET, &handshake) == -1) {
        printf("Error getting handshake lines %s - %s(%d).\n",
               bsdPath, strerror(errno), errno);
    }
    
    //printf("Handshake lines currently set to %d\n", handshake);
    printf("Handshake lines currently set to %d\n", handshake );
    
    mics = 1UL;
    
    if (ioctl(fileDescriptor, IOSSDATALAT, &mics) == -1) {
        // set latency to 1 microsecond
        printf("Error setting read latency %s - %s(%d).\n",
               bsdPath, strerror(errno), errno);
        goto error;
    }
    
    // Success
    return fileDescriptor;
    
error:
    if (fileDescriptor != -1) {
        close(fileDescriptor);
    }
    
    return -1;
    
}

char *logString(const char * str)
{
    static char buf[2048];
    char *ptr = buf;
    int i;
    
    *ptr = '\0';
    
    while ( *str )
    {
        if ( isprint(*str) )
        {
            *ptr++ = *str++;
        }
        
        else
        {
            switch (*str) {
                case ' ':
                    *ptr++ = *str;
                    break;
                    
                case 27:
                    *ptr++ = '\\';
                    *ptr++ = 'e';
                    break;
                    
                case '\t':
                    *ptr++ = '\\';
                    *ptr++ = 't';
                    break;
                    
                case '\n':
                    *ptr++ = '\\';
                    *ptr++ = 'n';
                    break;
                    
                case '\r':
                    *ptr++ = '\\';
                    *ptr++ = 'r';
                    break;
                
                default:
                    i = *str;
                    
                    (void)sprintf(ptr, "\\%03o", i);
                    ptr += 4;
                    break;
            }
            
            str++;
            
            
        }
        
        *ptr = '\0';
    }
    
    return buf;
}

char *logData(const char * str)
{
    static char buf[2048];
    char *ptr = buf;
    int i;
    
    *ptr = '\0';
    
    while ( *str )
    {
        if ( isprint(*str) )
        {
            *ptr++ = *str++;
        }
        
        *ptr = '\0';
    }
    
    return buf;
}

int initializeModem(int fileDescriptor)
{
    char buffer[256];
    char *bufPtr;
    ssize_t numBytes;
    int tries;
    bool result = false;
    
    for (tries = 1; tries <= kNumRetries; tries ++)
    {
        printf("Try #%d\n", tries );
        
        //Send an AT command to the modem.
        numBytes = write(fileDescriptor, kATCommandString , strlen(kATCommandString ));
        //numBytes = write(fileDescriptor, "/n" , 1 );
        
        //IF NUM BYTES = -1 write command failed.
        if ( numBytes == -1 )
        {
            printf("Error writing to modem - %s(%d).\n", strerror(errno), errno);
            continue;
        }
        //IF write command succeeded write to log.
        else
        {
            printf("Wrote %ld bytes \"%s\"\n", numBytes, logString(kATCommandString) );
        }
        
        if ( numBytes < strlen(kATCommandString))
        {
            continue;
        }
        
        printf("Looking for \"%s\"\n", logString(kOKResponseString) );
        
        // Read characters into our buffer until we get a CR or LF
        bufPtr = buffer;
        
        do
        {
            numBytes = read(fileDescriptor, bufPtr, &buffer[sizeof(buffer)] - bufPtr - 1);
            
            if (numBytes == -1) {
                printf("Error reading from modem - %s(%d).\n", strerror(errno), errno);
            }
            
            else if (numBytes > 0)
            {
                bufPtr += numBytes;
                if (*(bufPtr - 1) == '\n' || *(bufPtr - 1) == '\r') {
                    break;
                }
            }
            else {
                printf("Nothing read.\n");
            }
            
            
        } while ( numBytes > 0 );
        
        *bufPtr = '\0';
        
        printf("Read \"%s\"\n", logString(buffer) );
        
        printf("%s", buffer  );
        printf("%s", kOKResponseString );
        
        if ( strncmp(buffer, kOKResponseString, strlen(kOKResponseString)) == 0 ) {
            result = true;
            break;
        }
    }
    
    return result;
    
}


int writeSerialPort(int fileDescriptor, char * InString )
{
    ssize_t numBytes;
    
    numBytes = write(fileDescriptor, InString , strlen(InString ));
    
    //IF NUM BYTES = -1 write command failed.
    if ( numBytes == -1 )
    {
        printf("Error writing to modem - %s(%d).\n", strerror(errno), errno);
        return 4;
    }
    //IF write command succeeded write to log.
    else
    {
        printf("Wrote %ld bytes \"%s\"\n", numBytes, logString(InString) );
    }
    
    if ( numBytes < strlen(InString))
    {
        return 5;
    }

    return 0;

}

int readSerialPort( int fileDescriptor, char * dst )
{
    
    char buffer[1024];
    char *bufPtr;
    ssize_t numBytes;
    bool result = false;
    
    bufPtr = buffer;
    
    do
    {
        numBytes = read(fileDescriptor, bufPtr, &buffer[sizeof(buffer)] - bufPtr - 1);
        
        if (numBytes == -1) {
            printf("Error reading from modem - %s(%d).\n", strerror(errno), errno);
            return 4
        }
        
        else if (numBytes > 0)
        {
            bufPtr += numBytes;
            if (*(bufPtr - 1) == '\n' || *(bufPtr - 1) == '\r') {
                break;
            }
        }
        else {
            printf("Nothing read.\n");
            return 5;
        }
        
        
    } while ( numBytes > 0 );
    
    *bufPtr = '\0';
    
    printf("Read \"%s\"\n", buffer);
    
    strcpy(dst, buffer);

    return 0;
}


int closeSerialPort(int fileDescriptor)
{
    
    ssize_t numBytes;
    
    numBytes = write(fileDescriptor, kENDCommandString , strlen(kENDCommandString ));
    
    //IF NUM BYTES = -1 write command failed.
    if ( numBytes == -1 )
    {
        printf("Error writing to modem - %s(%d).\n", strerror(errno), errno);
        return -1;
    }
    //IF write command succeeded write to log.
    else
    {
        printf("Wrote %ld bytes \"%s\"\n", numBytes, logString(kENDCommandString));
    }
    
    if ( numBytes < strlen(kENDCommandString))
    {
        return -1;
    }
    
    // Block until all written output has been sent from the device.
    // Note that this call is simply passed on to the serial device driver.
    // See tcsendbreak(3) <x-man-page://3/tcsendbreak> for details.
    if (tcdrain(fileDescriptor) == -1) {
        printf("Error waiting for drain - %s(%d).\n",
               strerror(errno), errno);
    }
    
    // Traditionally it is good practice to reset a serial port back to
    // the state in which you found it. This is why the original termios struct
    // was saved.
    if (tcsetattr(fileDescriptor, TCSANOW, &gOriginalTTYAttrs) == -1) {
        printf("Error resetting tty attributes - %s(%d).\n",
               strerror(errno), errno);
    }
    
    
    
    return close(fileDescriptor);
}
# Wire
This is a "wire" between an application and an embedded device.  This library shall do the following things:

## Open and maintain a Serial Port using IOKit
1. Open a serial port.
1. Initialize the Modem.
1. Close the serial port.


## Read and Write messages to the serial port.
1. Read a message from the serial port.
1. Write a message to the serial port.

## Parse messages
1. Decompose data containing multiple parameters.

## Encode messages(i.e. base64)
1. encode messages to the serial port buffer.
1. decode messages from the serial port buffer.

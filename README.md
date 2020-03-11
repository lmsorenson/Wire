# Wire Lib
This library contains some procedures for reading from and writing to a serial port.  The purpose of this library is to handle communication with embedded devices. 

1. Installation: https://github.com/lmsorenson/Wire/blob/master/README.md#Installation
1. Use Cases: https://github.com/lmsorenson/Wire/blob/master/README.md#use-cases--requirements


## Installation
### Installing Dev Display
### Intalling and linking WireLib
### Building WireLib and Dev Display from Source
#### Step1 - Install Tools needed:
* Git 
(https://git-scm.com/download/mac)
* CMake 
(https://github.com/Kitware/CMake/releases/download/v3.17.0-rc2/cmake-3.17.0-rc2-Darwin-x86_64.dmg)
* Qt 
(https://www.qt.io/download-open-source)


### Step2 - Clone this repository:
1. Open a terminal on Mac and cd into the directory you'd like to clone the repo to.
> i.e clone to desktop.  
> cd ~/Desktop 
2. Run the clone command.
> git clone git-repo-url

### Step3 - run CMake
1. In the terminal set the working directory to WireLib or devDisplay
1. Make a directory called 'cmake' and make the new directory the working directory.
> mkdir cmake  
> cd cmake
1. Run the cmake command to generate the make files. (Making sure to pass in '..' as an argument so CMake will look for the CMakeLists.txt file in the previous directory)
> cmake ..
1. Run make to compile the static library.
> make

## Use Cases & Requirements
### Open and maintain a Serial Port using IOKit
1. Open a serial port.
1. Initialize the Modem.
1. Close the serial port.

### Read and Write messages to the serial port.
1. Read a message from the serial port.
1. Write a message to the serial port.

### Parse messages
1. Deserialize messages containing multiple parameters.

### Encode messages(i.e. base64)
1. encode messages to the serial port buffer.
1. decode messages from the serial port buffer.

# Steroido
A small OS running on a Microcontroller-Platform making cross-compilation easy.

## Build Status
Branch master: [![Travis Build Status](https://api.travis-ci.org/timoxd7/Steroido.svg?branch=master)](https://travis-ci.org/timoxd7/Steroido)

## Intention
Steriodo OS is intended to minimize the effort to port a software from one Microcontroller Platform to another. It is normally build on top of an existing Platform (e.g. Arduino or mbed) making it compatible with many microcontroller-boards.

## Compatibility
Currently, Steroido can be run on the following Platforms:
- Arduino
- Teensyduino
- mbed

## Features
Steriodo OS gives the following capabilities:
- Basic Input/Output of digital and analog Ports
- Basic Ticker System
- Timer
- Some Tools like delayedSwitch, memCpy, memSet...
- printf for all supported platforms
- vector for all supported platforms
- Callbacks
- CAN for all supported mbed and teensy devices (not arduino yet)

## Usage
Steroido OS in its basic usage is similar with Arduino:

    void setup() {
        // Setup is called once
    }
  
    void loop() {
        // Loop is called after setup infinitely
    }

You still need to include the Platform Header-File BEFORE including Steroido OS

    #include "Arduino.h" // for Arduino or Teensyduino
    #include "mbed.h" // for mbed

I recommend using PlatformIO to make the work on different Boards with the (almost) same software more easy.

## Precompiler Definitions
For some Microcontrollers, there have to be some definitions to make them work with Steroido

### Nucleo boards (tested)
For Nucleo development boards using mbed, nothing has to be defined.

For Nucleo development boards using Arduino, define the following (in code or with platformio):

    #define NUCLEO

Or in the platformio.ini config, add

    build_flags =
        -D NUCLEO

### Teensy Boards (Teensy 3.2 tested)
For Teensy boards using Arduino, define the following ether in code:

    #define TEENSY

Or in platformio.ini config

    build_flags =
        -D TEENSY

### Arduino AVR Boards (Arduino Nano tested)
For Arduino AVR Boards using Arduino, nothing has to be done :)

## Interface
For Short, the following Classes are defined across all platforms with an equal interface. Use the IDE of your choice (we use VS Code with PlatformIO) and use the builtin tools to show the Documentation and interface.

    // IO
    DigitalIn
    DigitalOut
    AnalogIn
    AnalogOut // same as PwmOut for Arduino, mbed uses real Analog
    PwmOut

    // Tools
    Timer // To measure time
    CircularBuffer // Nice static memory based buffer
    DelayedSwitch // For delayed turn on/turn off or simple button debounce
    CAN // For Teensy and mbed only
    vector // Array-based container

    // Functions
    mapC<datatype> // Map but you can specify the type of the value
    memCpy<datatype> // memcpy but you can specify the datatype
    memSet<datatype> // memset but you can specify the datatype
    printf // Write something easily to the Serial PC Connection

## Pin Names
The Pin names are always the same as for the given framework (Arduino == 1, 2, 3 ... A1, A2, A3...; mbed == PD_1, PD_2 ...)

## RTOS
The Doc for the RTOS will follow later. For now, just look in the code, its not much...
## Feature Request
For now, Steroido OS is developed for a Project, building a Race-Car for the Formula Student to run all control-devices with the same software, even there are used many different microcontrollers. Because we don't have much time, i will (for now) only implement features, i really need. Later, after finishing this, i will also implement different Features, maybe direct hardware access, other Optimizations and so on. 

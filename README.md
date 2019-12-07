# Steroido
A small OS running on a Microcontroller-Platform making cross-compilation easy.

## Build Status
Branch master: [![Travis Build Status](https://api.travis-ci.org/timoxd7/Steroido.svg?branch=master)](https://travis-ci.org/timoxd7/Steroido)

## Intention
Steriodo OS is intended to minimize the effort to port a software from one Microcontroller Platform to another. It is normally build on top of an existing Platform (e.g. Arduino or mbed) making it compatible with many microcontroller-boards.

## Interface
Later, you will see the Interface to use Sterido here.

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

## Feature Request
For now, Steroido OS is developed for a Project building a Race-Car for the Formula Student to run all control-devices with the same software, even there are used many different microcontrollers. Because we don't have much time, i will (for now) only implement features, i really need. Later, after finishing this, i will also implement different Features, maybe direct hardware access, other Optimizations and so on. 

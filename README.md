# LetMeCreate IoT

## Overview

A collection of wrappers for the Contiki interfaces made to ease the development of applications for the Mikro-e PIC32MX clicker.

Supported interfaces:
  - Network
  - I2C
  - SPI
  - GPIO & interrupts

Supported clicks:
  - Color
  - Thermo3
  - Proximity
  - Motion
  - Relay
  - Relay2
  - Joystick
  - Air Quality
  - Accel (SPI only)
  - 8x8R (Led Matrix)

## Installation

```sh
$ ./install.sh path/to/contiki
```

This will modify the Contiki files to add LMC on top of them. Additionally, a symbolic link to Contiki directory will be created for the example Makefiles.

To verify whether installation worked navigate to the examples directory, pick an example of your choice and run

```sh
make
```

## Update

To update the library run:

```sh
git pull
./install.sh
```

As long as a symbolic link to contiki directory exists the installation script will update all files.

## Development

To use include the appropriate header files, eg.:

```C
#include <letmecreate/core/spi.h>
#include <letmecreate/click/accel.h>
```

Check the examples directory for examples on how to use Clicker wrappers & other library functionality.

### Comptability with CI40 LMC

The interfaces are made to be as compatible with the [CI40 LMC](https://github.com/francois-berder/LetMeCreate) version as possible to ensure the code is portable. Use following defines to write multi-platform Clicker drivers:

```C
#ifdef CONTIKI
/** Code for Contiki here */
#else
/** Code for CI40 here */
endif
```

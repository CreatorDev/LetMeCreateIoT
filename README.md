# LetMeCreate IoT

## Overview

A collection of wrappers for the Contiki interfaces made to speed up the development of applications for the Mikro-e PIC32MX clicker.

Wrappers:
  - I2C
  - SPI
  - GPIO
  - Interrupts
  - ADC
  - Network
  - Debug over 6lowpan

Supported clicks:
  - Relay
  - Thermo3
  - Motion
  - Accel (SPI only)
  - Air Quality
  - Color
  - Joystick
  - 8x8R (Led Matrix)
  - Proximity
  - Relay2

## Installation

```sh
$ ./install.sh path/to/contiki
```

This will modify the Contiki files to add LMCIoT on top of them. Additionally, a symbolic link to Contiki directory will be created for the example Makefiles.

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

To use include the appropriate header files in your project, eg.:

```C
#include "letmecreate/core/spi.h"
#include "letmecreate/click/accel.h"
```

For reference how to use the different interfaces or click wrappers refer to a wide selection of examples in the examples directory.

### Debug

LMCIoT offers debug capabilities over 6lowpan. For specific example of usage check examples/debug and scripts/debug_server.py.

### Compatibility with CI40 LMC

The interfaces are made to be as compatible with the [CI40 LMC](https://github.com/francois-berder/LetMeCreate) version as possible to ensure the code is portable. Use following defines to write multi-platform Clicker drivers:

```C
#ifdef CONTIKI
/** Code for Contiki here */
#else
/** Code for CI40 here */
endif
```

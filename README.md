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
  - Relay2
  - Joystick
  - Air Quality
  - Accel (SPI only)
  - 8x8R (Led Matrix)

## Installation

```sh
$ ./install.sh /path/to/contiki
```

This will modify the Contiki files to add LMC on top of them.

To use simply include the appropriate header files, eg.:

```C
#include <letmecreate/core/spi.h>
#include <letmecreate/click/accel.h>
```

Check for examples in the examples directory.

## Comptability with CI40 LMC

The interfaces are made to be as compatible with the [CI40 LMC](https://github.com/francois-berder/LetMeCreate) version as possible to ensure the code is portable. Use following defines to write multi-platform Clicker drivers:

```C
#ifdef CONTIKI
/** Code for Contiki here */
#else
/** Code for CI40 here */
endif
```

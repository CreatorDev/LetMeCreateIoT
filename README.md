# LetMeCreate IoT

## Overview

A collection of wrappers for the Contiki interfaces made to ease the development of applications for the Mikro-e PIC32MX clicker.

Supported interfaces:
  - Network
  - I2C
  - SPI

Supported clicks:
  - Accel

## Installation

The library is currently installed via a Bash script which modifies Contiki makefile and inserts the files into its directory. Run ./install.sh contiki/directory to install. To use include "lmc/core/\<interface\>.h" or "lmc/click/\<click\>.h" headers.

## Links

For the CI40 version of the library see [here](https://github.com/francois-berder/LetMeCreate)

You can find various Contiki and CI projects [here](https://github.com/mtusnio/ci40projects)


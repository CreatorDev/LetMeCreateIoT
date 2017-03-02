![logo](https://static.creatordev.io/logo-md-s.svg)
# LetMeCreateIoT

**Master**  
[![Build Status](https://travis-ci.org/CreatorDev/LetMeCreateIoT.svg?branch=master)](https://travis-ci.org/CreatorDev/LetMeCreateIoT)  
**Dev**  
[![Build Status](https://travis-ci.org/CreatorDev/LetMeCreateIoT.svg?branch=dev)](https://travis-ci.org/CreatorDev/LetMeCreateIoT)

## Introduction

A collection of wrappers for the Contiki interfaces made to speed up the development of applications 
for the Mikro-e PIC32MX clicker.

|Wrappers||
|:------------| :-------------------|
|I²C|SPI|
|GPIO|GPIO Monitor|
|ADC|UDP/TCP & network helpers|
|Debug over 6lowpan|LEDs|
|UART|PWM|

All drivers are checked out from [LetMeCreate 1.5.1](https://github.com/CreatorDev/LetMeCreate/tree/v1.5.1) and
Contiki-specific patches are applied on top of them..

|Drivers|||
|:------------|:-------------------|:-------------------|
|7Seg|8x8R (Led Matrix)|Accel|
|ADC|Air quality|Alcohol|
|Alphanum|Bargraph|CO|
|Color|Color2|EVE|
|Fan|GYRO|IR distance|
|IR eclipse|Joystick|Light|
|LIN Hall|Lora|Motion|
|OLED|Opto|Proximity|
|Relay (partial support)|Relay2|Relay4 (partial support)|
|RTC|Thermo3|UNI Hall|
|Weather|||

## Installation and updating

### Installation

By default the library should be already in the [Creator branch of CreatorDev Contiki](https://github.com/CreatorDev/contiki/tree/creator).
If you want to use it with Contiki that does not have LMCIoT by default use following commands:

```sh
cd contiki/apps
git clone https://github.com/CreatorDev/LetMeCreateIoT/ letmecreateiot
```

To use in your application add this to your application's Makefile:

```make
APPS += letmecreateiot
```

The library will automatically update all driver feeds when make is run.

## Development

To use include the appropriate header files in your project, eg.:

```C
#include "letmecreate/core/spi.h"
#include "letmecreate/click/accel.h"
```

For reference how to use the different interfaces or click wrappers refer to a wide selection of 
examples in the examples directory.

### CI40 UDP server & client

In addition to wrappers for common networking functionality the repository contains two Python scripts
that can be used as 6lowpan client/server. Both of them can be found in the scripts directory, and
can be freely modifid to send and receive application data. For more help run:

```sh
python3 server.py -h
python3 client.py -h
```

If Python3 is not installed use [opkg](https://docs.creatordev.io/ci40/guides/openwrt-platform/#using-opkg)
to install the package:

```sh
opkg install python3
```

### Debug

LMCIoT offers debug capabilities over 6lowpan. This is achieved by adding a new macro, PRINTF, which 
sends data over 6lowpan every time it is needed. To enable first include the DEBUG_IP compilation 
define in the Makefile:

```Makefile
CFLAGS += -DDEBUG_IP=6lowpan:ip:of:ci40
```

By default data is not only sent over 6lowpan but also printed over UART. To disable serial logging use:

```Makefile
CFLAGS += -DDEBUG_IP=6lowpan:ip:of:ci40 -DDEBUG_NO_SERIAL
```

Then use the INIT_NETWORK_DEBUG and PRINTF macros in code:
```C
/* ... */
#include "letmecreate/core/debug.h"

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();
    INIT_NETWORK_DEBUG();
    {
        PRINTF("Start!\n");
        /* ... */
```
Finally, copy scripts/debug_server.py to a CI40 with Python3 installed and run:
```sh
python3 debug_server.py
```

To disable debugging simply remove DEBUG_IP from CFLAGS. In this case INIT_NETWORK_DEBUG will 
resolve to no code at all and PRINTF will act as the standard printf function. For a full example 
program visit [examples/debug](https://github.com/CreatorDev/LetMeCreateIoT/tree/master/examples/debug). 
Additionally, all other examples can be used with network debug enabled by following the above steps.

### Compatibility with LMC

The interfaces are made to be as compatible with [LetMeCreate](https://github.com/CreatorDev/LetMeCreate) 
as much as possible to ensure the code is portable. When adding support for a new Click use its LMC
version, modify for use with LMCIoT and generate a patch, then place it in the
feeds/patches/ directory.

### Known issues

- At the moment the new udp connection sends dummy data to force router solicitation as soon as 
possible.

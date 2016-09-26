# LetMeCreateIoT

## Introduction

A collection of wrappers for the Contiki interfaces made to speed up the development of applications for the Mikro-e PIC32MX clicker.

Wrappers:
  - I2C
  - SPI
  - GPIO
  - Interrupts
  - ADC
  - Network
  - Debug over 6lowpan
  - Power saving control

Supported clicks:
  - Relay
  - Thermo3
  - Motion
  - Accel (SPI only)
  - Air Quality
  - Alcohol
  - Color
  - Joystick
  - 8x8R (Led Matrix)
  - Proximity
  - Relay2

### Installation

```sh
$ ./install.sh path/to/contiki
```

This will modify the Contiki files to add LMCIoT on top of them. Additionally, a symbolic link to Contiki directory will be created for the example Makefiles.

To verify whether installation worked navigate to the examples directory, pick an example of your choice and run

```sh
make
```

### Updating

To update the library run:

```sh
git pull
./install.sh
```

As long as a symbolic link to Contiki directory exists the installation script will update all files.

## Development

To use include the appropriate header files in your project, eg.:

```C
#include "letmecreate/core/spi.h"
#include "letmecreate/click/accel.h"
```

For reference how to use the different interfaces or click wrappers refer to a wide selection of examples in the examples directory.

### UDP server

In addition to wrappers for common networking functionality you can also find a simple UDP server to modify for any of your projects. To use it
modify server.py to reflect the structure of your data and copy it to the CI40. Make sure Python3 is installed then run the server:

```python
python3 server.py
```

### Debug

LMCIoT offers debug capabilities over 6lowpan. This is achieved by adding a new macro, PRINTF, which sends data over 6lowpan every time
it is needed. To enable first include the DEBUG_IP compilation define in the Makefile:

```Makefile
CFLAGS += -DDEBUG_IP=6lowpan:ip:of:ci40
```

Then use the INIT_NETWORK_DEBUG and PRINTF macros in code:
```C
/* ... */
#include "letmecreate/core/network.h"

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

To disable debugging simply remove DEBUG_IP from CFLAGS. In this case INIT_NETWORK_DEBUG will resolve to no code at all and PRINTF will act as the standard printf function. 
For a full example program visit examples/debug. Additionally, a few other examples also incorporate this debug functionality.

### Compatibility with LMC

The interfaces are made to be as compatible with [LMC](https://github.com/francois-berder/LetMeCreate) as possible to ensure the code is portable. Use following defines to write multi-platform Clicker drivers:

```C
#ifdef CONTIKI
/** Code for Contiki here */
#else
/** Code for CI40 here */
endif
```

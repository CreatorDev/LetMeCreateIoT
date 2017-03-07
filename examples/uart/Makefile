CONTIKI=../../../../../../

TARGET = mikro-e
CONTIKI_WITH_IPV6 = 1
CONTIKI_WITH_RPL = 0
USE_CA8210 = 1
USE_SERIAL_PADS = 1
VERSION? = $(shell git describe --abbrev=4 --dirty --always --tags)

APPS += letmecreateiot

CFLAGS += -DDEBUG_IP=fe80::219:f5ff:fe89:bfc
CFLAGS += -DVERSION=$(VERSION)
CFLAGS += -Wall -Wno-pointer-sign
CFLAGS += -I $(CONTIKI)/platform/$(TARGET)
CFLAGS += -fno-short-double
# Needed for UART to work with USE_SERIAL_PADS equal to 1
CONTIKI_PLAT_DEFS += -D __USE_UART_PORT3__
LDFLAGS += -Wl,--defsym,_min_heap_size=32000

SMALL=0

all: main
	xc32-bin2hex main.$(TARGET)

include $(CONTIKI)/Makefile.include

CONTIKI=../../../../../../

TARGET = mikro-e
VERSION? = $(shell git describe --abbrev=4 --dirty --always --tags)

CONTIKI_WITH_IPV6 = 1
CONTIKI_WITH_RPL = 0
USE_SERIAL_PADS = 1

APPS += letmecreateiot

CFLAGS += -DVERSION=$(VERSION)
CFLAGS += -Wall -Wno-pointer-sign
CFLAGS += -I $(CONTIKI)/platform/$(TARGET)
CFLAGS += -fno-short-double
LDFLAGS += -Wl,--defsym,_min_heap_size=32000

SMALL=0

all: main
	xc32-bin2hex main.$(TARGET)

include $(CONTIKI)/Makefile.include

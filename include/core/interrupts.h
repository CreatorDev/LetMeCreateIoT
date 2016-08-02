#ifndef H_CORE_INTERRUPTS
#define H_CORE_INTERRUPTS

#include <stdint.h>

int interrupt_configure(void (*callback)(uint8_t));

#endif

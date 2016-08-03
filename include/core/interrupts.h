#ifndef H_CORE_INTERRUPTS
#define H_CORE_INTERRUPTS

#include <stdint.h>

/** Event on GPIO */
enum GPIO_EVENT {
    GPIO_RAISING   = 0x01,
    GPIO_FALLING   = 0x02,
    GPIO_EDGE      = 0x03
};

int interrupt_configure(void (*callback)(uint8_t));

#endif

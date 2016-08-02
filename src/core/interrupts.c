#include <letmecreate/core/interrupts.h>

#include <stdio.h>
#include <contiki.h>
#include "dev/interrupts.h"
#include <letmecreate/core/gpio.h>

static void (*interrupt_callback)(uint8_t) = NULL;

static void interrupt_handler()
{
    if(interrupt_callback == NULL)
    {
        fprintf(stderr, "interrupts: Callback called with null handler\n");
        return;
    }

    uint8_t value;
    if(gpio_get_value(INT, &value) < 0)
    {
        fprintf(stderr, "interrupts: Failed to get interrupt value\n");
        return;
    }

    interrupt_callback(value);
}

int interrupt_configure(void (*callback)(uint8_t))
{
    if(callback == NULL)
    {
        if(interrupt_isr != NULL)
        {
            INTERRUPT_IRQ_DISABLE();
            interrupt_isr = NULL;
            interrupt_callback = NULL;
        }
        return 0;
    }

    INTERRUPT_IRQ_INIT();
    INTERRUPT_IRQ_ENABLE();
    interrupt_callback = callback;
    interrupt_isr = interrupt_handler;

    return 0;
}

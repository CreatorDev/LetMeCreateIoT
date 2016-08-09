#include "letmecreate/core/adc.h"

#include <stdio.h>
#include <pic32_gpio.h>

#include "letmecreate/core/common.h"

int adc_get_value(uint8_t mikrobus_index, float * value)
{
    if(mikrobus_index != MIKROBUS_1)
    {
        fprintf(stderr, "ADC: Unknown Mikrobus index passed\n");
        return -1;
    }

    GPIO_CONFIGURE_AS_ANALOG(G, 9);
    GPIO_CONFIGURE_AS_INPUT(G, 9);

    return GPIO_VALUE(G, 9);
}

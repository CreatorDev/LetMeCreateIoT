#include "letmecreate/core/adc.h"

#include <stdio.h>
#include <pic32_gpio.h>

#include "letmecreate/core/common.h"

int adc_get_value(uint8_t mikrobus_index, float * value)
{
    int tmp_val;

    if(mikrobus_index != MIKROBUS_1)
    {
        fprintf(stderr, "ADC: Unknown Mikrobus index passed\n");
        return -1;
    }

    if(value == NULL)
    {
        fprintf(stderr, "ADC: Cannot write to null variable\n");
        return -1;
    }

    GPIO_CONFIGURE_AS_ANALOG(G, 9);
    GPIO_CONFIGURE_AS_INPUT(G, 9);

    tmp_val = GPIO_VALUE(G, 9);

    return 5.0f * ((float)tmp_val/1023.0f);
}

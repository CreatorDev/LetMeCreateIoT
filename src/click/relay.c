#include <stdio.h>
#include "letmecreate/core/common.h"
#include "letmecreate/core/gpio.h"
#include "letmecreate/click/relay.h"


int relay_click_enable_relay_1(uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
        if (!gpio_init(MIKROBUS_1_PWM)
        &&  !gpio_set_direction(MIKROBUS_1_PWM, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_1_PWM, 1))
            return 0;
#ifndef CONTIKI
    case MIKROBUS_2:
        if (!gpio_init(MIKROBUS_2_PWM)
        &&  !gpio_set_direction(MIKROBUS_2_PWM, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_2_PWM, 1))
            return 0;
#endif
        default:
            fprintf(stderr, "relay: Invalid mikrobus index.\n");
            return -1;
    }

    return -1;
}

int relay_click_disable_relay_1(uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
        if (!gpio_init(MIKROBUS_1_PWM)
        &&  !gpio_set_direction(MIKROBUS_1_PWM, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_1_PWM, 0))
            return 0;
#ifndef CONTIKI
    case MIKROBUS_2:
        if (!gpio_init(MIKROBUS_2_PWM)
        &&  !gpio_set_direction(MIKROBUS_2_PWM, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_2_PWM, 0))
            return 0;
#endif
        default:
            fprintf(stderr, "relay: Invalid mikrobus index.\n");
            return -1;
    }

    return -1;
}

int relay_click_enable_relay_2(uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
        if (!gpio_init(MIKROBUS_1_CS)
        &&  !gpio_set_direction(MIKROBUS_1_CS, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_1_CS, 1))
            return 0;
#ifndef CONTIKI
    case MIKROBUS_2:
        if (!gpio_init(MIKROBUS_2_CS)
        &&  !gpio_set_direction(MIKROBUS_2_CS, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_2_CS, 1))
            return 0;
#endif
        default:
            fprintf(stderr, "relay: Invalid mikrobus index.\n");
            return -1;
    }

    return -1;
}

int relay_click_disable_relay_2(uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
        if (!gpio_init(MIKROBUS_1_CS)
        &&  !gpio_set_direction(MIKROBUS_1_CS, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_1_CS, 0))
            return 0;
#ifndef CONTIKI
    case MIKROBUS_2:
        if (!gpio_init(MIKROBUS_2_CS)
        &&  !gpio_set_direction(MIKROBUS_2_CS, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_2_CS, 0))
            return 0;
#endif
        default:
            fprintf(stderr, "relay: Invalid mikrobus index.\n");
            return -1;
    }

    return -1;
}

#include <stdio.h>
#include <letmecreate/core/led.h>
#include <core/dev/leds.h>
#include "contiki-conf.h"


int led_init(void)
{
    led_switch_off(ALL_LEDS);
    return 0;
}

int led_switch_on(uint8_t mask)
{
    return led_set(mask, 0xFF);
}

int led_switch_off(uint8_t mask)
{
    return led_set(mask, 0);
}

int led_set(uint8_t mask, uint8_t value)
{
    unsigned char state = leds_get();
    if (mask & LED_0)
    {
        if (value & LED_0)
            state |= LED1;
        else
            state &= ~LED1;
    }
    if (mask & LED_1)
    {
        if (value & LED_1)
            state |= LED2;
        else
            state &= ~LED2;
    }

    leds_set(state);

    return 0;
}

int led_configure_on_off_mode(uint8_t mask)
{
    /* Nothing to do, on clickers LED's are always in ON_OFF_MODE */
    return 0;
}

int led_configure_timer_mode(uint8_t mask)
{
    fprintf(stderr, "led: Timer mode is not supported.\n");
    return -1;
}

int led_get_mode(uint8_t led_index, uint8_t *led_mode)
{
    if (led_mode == NULL)
    {
        fprintf(stderr, "led: led mode must not be null.\n");
        return -1;
    }

    switch (led_index) {
    case LED_0:
    case LED_1:
        *led_mode = ON_OFF_MODE; /* On clicker, only ON_OFF_MODE is supported */
        break;
    default:
        fprintf(stderr, "led: Invalid led index.\n");
        return -1;
    }

    return 0;
}

int led_set_delay(uint8_t mask, uint32_t delay_on, uint32_t delay_off)
{
    fprintf(stderr, "led: Timer mode is not supported");
    return -1;
}

int led_release(void)
{
    led_switch_off(ALL_LEDS);
    return 0;
}

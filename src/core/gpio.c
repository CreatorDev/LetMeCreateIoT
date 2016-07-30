#include <letmecreate/core/gpio.h>

#include <stdio.h>
#include <pic32_gpio.h>

#define PIN_FUNCTION(func, gpio_pin)   {        \
    switch(gpio_pin)                            \
    {                                           \
        case(AN):                               \
        func(G, 9);                             \
        break;                                  \
        case(RST):                              \
        func(D, 6);                             \
        break;                                  \
        case(PWM):                              \
        func(B, 8);                             \
        break;                                  \
        case(INT):                              \
        func(D, 0);                             \
        break;                                  \
        default:                                \
        return -1;                              \
     }                                          \
  }


/** AN, RST, PWM, INT */
static uint8_t pin_direction[4] = { 0 };

int gpio_init(uint8_t gpio_pin)
{
    return gpio_set_direction(gpio_pin, GPIO_INPUT);
}


int gpio_set_direction(uint8_t gpio_pin, uint8_t dir)
{
    if(dir == GPIO_OUTPUT)
    {
        PIN_FUNCTION(GPIO_CONFIGURE_AS_OUTPUT, gpio_pin);
    }
    else if(dir == GPIO_INPUT)
    {
        PIN_FUNCTION(GPIO_CONFIGURE_AS_INPUT, gpio_pin);
    }
    else
    {
        fprintf(stderr, "GPIO: Unknown direction\n");
        return -1;
    }

    switch(gpio_pin)
    {
        case AN:
        pin_direction[0] = dir;
        break;
        case RST:
        pin_direction[1] = dir;
        break;
        case PWM:
        pin_direction[2] = dir;
        break;
        case INT:
        pin_direction[3] = dir;
        break;
        default:
        fprintf(stderr, "GPIO: Unknown pin\n");
        return -1;
    }
    return 0;
}

int gpio_get_direction(uint8_t gpio_pin, uint8_t *dir)
{
    if(dir == NULL)
    {
        fprintf(stderr, "GPIO: Cannot write to null pointer\n");
        return -1;
    }

    switch(gpio_pin)
    {
        case AN:
        *dir = pin_direction[0];
        break;
        case RST:
        *dir = pin_direction[1];
        break;
        case PWM:
        *dir = pin_direction[2];
        break;
        case INT:
        *dir = pin_direction[3];
        break;
        default:
        fprintf(stderr, "GPIO: Unknown pin\n");
        return -1;
    }

    return 0;
}

int gpio_set_value(uint8_t gpio_pin, uint8_t value)
{
    if(value == 0)
    {
        PIN_FUNCTION(GPIO_CLR, gpio_pin);
    }
    else if(value == 1)
    {
        PIN_FUNCTION(GPIO_SET, gpio_pin);
    }
    else
    {
        fprintf(stderr, "GPIO: Unknown gpio value\n");
        return -1;
    }

    return 0;
}

int gpio_get_value(uint8_t gpio_pin, uint8_t *value)
{
    if(value == NULL)
    {
        fprintf(stderr, "GPIO: Cannot write to null pointer\n");
        return -1;
    }

    PIN_FUNCTION(*value = GPIO_VALUE, gpio_pin);
    return 0;
}

int gpio_release(uint8_t gpio_pin)
{
    /** Nothing here, kept for comptability */
    return 0;
}

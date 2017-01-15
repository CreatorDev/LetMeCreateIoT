#include <letmecreate/core/gpio.h>

#include <p32xxxx.h>

#include <stdio.h>
#include <pic32_gpio.h>

#define PIN_FUNCTION(func, gpio_pin)   {        \
    switch(gpio_pin)                            \
    {                                           \
        case(GPIO_AN):                          \
        func(G, 9);                             \
        break;                                  \
        case(GPIO_RST):                         \
        func(D, 6);                             \
        break;                                  \
        case(GPIO_PWM):                         \
        func(B, 8);                             \
        break;                                  \
        case(GPIO_INT):                         \
        func(D, 0);                             \
        break;                                  \
        case(GPIO_CS):                          \
        func(E, 5);                             \
        break;                                  \
        default:                                \
        return -1;                              \
     }                                          \
  }


/** AN, RST, PWM, INT, CS */
static uint8_t pin_direction[5] = { 0 };

int gpio_init(uint8_t gpio_pin)
{
    switch(gpio_pin)
    {
        case GPIO_AN:
        GPIO_CONFIGURE_AS_DIGITAL(G, 9);
        break;
        case GPIO_PWM:
        GPIO_CONFIGURE_AS_DIGITAL(B, 8);
        break;
        case GPIO_CS:
        GPIO_CONFIGURE_AS_DIGITAL(E, 5);
        default:
        fprintf(stderr, "GPIO: Unknown pin\n");
        return -1;
    }

    PIN_FUNCTION(GPIO_CLR, gpio_pin);

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
        case GPIO_AN:
        pin_direction[0] = dir;
        break;
        case GPIO_RST:
        pin_direction[1] = dir;
        break;
        case GPIO_PWM:
        pin_direction[2] = dir;
        break;
        case GPIO_INT:
        pin_direction[3] = dir;
        break;
        case GPIO_CS:
        pin_direction[4] = dir;
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
        case GPIO_AN:
        *dir = pin_direction[0];
        break;
        case GPIO_RST:
        *dir = pin_direction[1];
        break;
        case GPIO_PWM:
        *dir = pin_direction[2];
        break;
        case GPIO_INT:
        *dir = pin_direction[3];
        break;
        case GPIO_CS:
        *dir = pin_direction[4];
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

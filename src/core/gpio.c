#include <letmecreate/core/gpio.h>

#include <p32xxxx.h>

#include <stdio.h>
#include <pic32_gpio.h>

#include <letmecreate/core/common.h>

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


static const uint8_t pin_lookup[MIKROBUS_COUNT][TYPE_COUNT] = {
 { MIKROBUS_1_AN, MIKROBUS_1_RST, MIKROBUS_1_PWM, MIKROBUS_1_INT, MIKROBUS_1_CS },
};

int gpio_init(uint8_t gpio_pin)
{
    switch(gpio_pin)
    {
        case GPIO_INT:
        case GPIO_RST:
        case GPIO_PWM:
        break;
        case GPIO_AN:
        GPIO_CONFIGURE_AS_DIGITAL(G, 9);
        break;
        case GPIO_CS:
        GPIO_CONFIGURE_AS_DIGITAL(E, 5);
        break;
        default:
        fprintf(stderr, "GPIO: Unknown pin\n");
        return -1;
    }

    PIN_FUNCTION(GPIO_CLR, gpio_pin);

    return gpio_set_direction(gpio_pin, GPIO_INPUT);
}

int gpio_get_pin(uint8_t mikrobus_index, uint8_t pin_type, uint8_t * pin)
{
    if (pin == NULL)
    {
        fprintf(stderr, "gpio: Pin cannot be null\n");
        return -1;
    }

    if (check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "gpio: Invalid mikrobus index\n");
        return -1;
    }

    if (pin_type >= TYPE_COUNT)
    {
        fprintf(stderr, "gpio: Invalid pin type\n");
        return -1;
    }

    *pin = pin_lookup[mikrobus_index][pin_type];
    return 0;
}

int gpio_get_type(uint8_t gpio_pin, uint8_t *pin_type)
{
    if (pin_type == NULL)
    {
        fprintf(stderr, "gpio: Pin type cannot be null\n");
        return -1;
    }

    switch(gpio_pin)
    {
    case GPIO_INT:
        *pin_type = TYPE_INT;
        break;
    case GPIO_RST:
        *pin_type = TYPE_RST;
        break;
    case GPIO_PWM:
        *pin_type = TYPE_PWM;
        break;
    case GPIO_AN:
        *pin_type = TYPE_AN;
        break;
    case GPIO_CS:
        *pin_type = TYPE_CS;
        break;
    default:
        fprintf(stderr, "GPIO: Unknown pin\n");
        return -1;
    }

    return 0;
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
        *dir = TRISG & (1 << 9) ? GPIO_INPUT : GPIO_OUTPUT;
        break;
        case GPIO_RST:
        *dir = TRISD & (1 << 6) ? GPIO_INPUT : GPIO_OUTPUT;
        break;
        case GPIO_PWM:
        *dir = TRISF & (1 << 3) ? GPIO_INPUT : GPIO_OUTPUT;
        break;
        case GPIO_INT:
        *dir = TRISD & 1 ? GPIO_INPUT : GPIO_OUTPUT;
        break;
        case GPIO_CS:
        *dir = TRISE & (1 << 5) ? GPIO_INPUT : GPIO_OUTPUT;
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

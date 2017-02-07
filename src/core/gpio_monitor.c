#include <letmecreate/core/gpio_monitor.h>

#include <stdio.h>
#include <contiki.h>
#include <letmecreate/core/common.h>
#include <letmecreate/core/gpio.h>

#include "button-sensor.h"

#define INTERRUPT_PRIORITY 6
#define INTERRUPT_SUBPRIORITY 0

#define CALLBACK_COUNT          (20)

struct interrupt_callback
{
    uint32_t callback_ID;
    uint8_t gpio_pin;
    uint8_t event_mask;
    void (*callback)(uint8_t);
};

static uint32_t current_callback_ID = 0;
static struct interrupt_callback callbacks[CALLBACK_COUNT];

/* Keep track of how many callbacks are attached to each gpio.
 * On the clicker, there are TYPE_COUNT (=5) gpio's that can be controlled.
 *
 * This is used to enable interrupts only when the first callback is attached
 * to a particular gpio. Similarly, if all callbacks are removed from a gpio,
 * interrupts are disabled for this gpio.
 */
static uint8_t callback_count_per_pin[TYPE_COUNT];

static void handle_interrupt(uint8_t gpio_pin, uint8_t value)
{
    uint32_t i;
    uint8_t event_type = value ? GPIO_RAISING : GPIO_FALLING;

    for (i = 0; i < CALLBACK_COUNT; ++i) {
        if (!callbacks[i].callback)
            continue;
        if (callbacks[i].gpio_pin != gpio_pin)
            continue;

        if(callbacks[i].event_mask & event_type)
            callbacks[i].callback(event_type);
    }
}

static int enable_interrupt(uint8_t gpio_pin)
{
    switch(gpio_pin) {
    case GPIO_AN:
        CNCONGSET = _CNCONG_ON_MASK;
        IEC1SET = _IEC1_CNGIE_MASK;
        IFS1CLR = _IFS1_CNGIF_MASK;
        (void)PORTG;
        CNENGSET = _CNENG_CNIEG9_MASK;
        break;
    case GPIO_PWM:
        CNCONBSET = _CNCONB_ON_MASK;
        IEC1SET = _IEC1_CNBIE_MASK;
        IFS1CLR = _IFS1_CNBIF_MASK;
        (void)PORTB;
        CNENBSET = _CNENB_CNIEB8_MASK;
        break;
    case GPIO_INT:
        CNCONDSET = _CNCOND_ON_MASK;
        IEC1SET = _IEC1_CNDIE_MASK;
        IFS1CLR = _IFS1_CNDIF_MASK;
        (void)PORTD;
        CNENDSET = _CNEND_CNIED0_MASK;
        break;
    case GPIO_RST:
        CNCONDSET = _CNCOND_ON_MASK;
        IEC1SET = _IEC1_CNDIE_MASK;
        IFS1CLR = _IFS1_CNDIF_MASK;
        (void)PORTD;
        CNENDSET = _CNEND_CNIED6_MASK;
        break;
    case GPIO_CS:
        CNCONESET = _CNCONE_ON_MASK;
        IEC1SET = _IEC1_CNEIE_MASK;
        IFS1CLR = _IFS1_CNEIF_MASK;
        (void)PORTE;
        CNENESET = _CNENE_CNIEE5_MASK;
        break;
    default:
        fprintf(stderr, "gpio_monitor: Invalid pin");
        return -1;
    }

    return 0;
}

static int disable_interrupt(uint8_t gpio_pin)
{
    switch(gpio_pin) {
    case GPIO_AN:
        CNCONGCLR = _CNCONG_ON_MASK;
        IEC1CLR = _IEC1_CNGIE_MASK;
        IFS1CLR = _IFS1_CNGIF_MASK;
        (void)PORTG;
        CNENGCLR = _CNENG_CNIEG9_MASK;
        break;
    case GPIO_PWM:
        CNCONBCLR = _CNCONB_ON_MASK;
        IEC1CLR = _IEC1_CNBIE_MASK;
        IFS1CLR = _IFS1_CNBIF_MASK;
        (void)PORTB;
        CNENBCLR = _CNENB_CNIEB8_MASK;
        break;
    case GPIO_INT:
        CNCONDCLR = _CNCOND_ON_MASK;
        IEC1CLR = _IEC1_CNDIE_MASK;
        IFS1CLR = _IFS1_CNDIF_MASK;
        (void)PORTD;
        CNENDCLR = _CNEND_CNIED0_MASK;
        break;
   case GPIO_RST:
        CNCONDCLR = _CNCOND_ON_MASK;
        IEC1CLR = _IEC1_CNDIE_MASK;
        IFS1CLR = _IFS1_CNDIF_MASK;
        (void)PORTD;
        CNENDCLR = _CNEND_CNIED6_MASK;
        break;
    case GPIO_CS:
        CNCONECLR = _CNCONE_ON_MASK;
        IEC1CLR = _IEC1_CNEIE_MASK;
        IFS1CLR = _IFS1_CNEIF_MASK;
        (void)PORTE;
        CNENECLR = _CNENE_CNIEE5_MASK;
        break;
    default:
        fprintf(stderr, "gpio_monitor: Invalid pin\n");
        return -1;
    }

    return 0;
}

static int convert_gpio_pin(uint8_t gpio_pin)
{
    switch (gpio_pin) {
    case GPIO_AN:
        return TYPE_AN;
    case GPIO_RST:
        return TYPE_RST;
    case GPIO_PWM:
        return TYPE_PWM;
    case GPIO_INT:
        return TYPE_INT;
    case GPIO_CS:
        return TYPE_CS;
    default:
        return -1;
    }
}

ISR(_CHANGE_NOTICE_VECTOR) {
    if(BUTTON1_CHECK_IRQ())
    {
        button1_isr();
    }
    else if(BUTTON2_CHECK_IRQ())
    {
        button2_isr();
    }

    if(IFS1bits.CNGIF & CNSTATGbits.CNSTATG9)
    {
        handle_interrupt(GPIO_AN, GPIO_VALUE(G, 9));
        IFS1CLR = _IFS1_CNGIF_MASK;
        CNSTATGCLR = _CNSTATG_CNSTATG9_MASK;
    }
    if(IFS1bits.CNDIF & CNSTATDbits.CNSTATD0)
    {
        handle_interrupt(GPIO_INT, GPIO_VALUE(D, 0));
        IFS1CLR = _IFS1_CNDIF_MASK;
        CNSTATDCLR = _CNSTATD_CNSTATD0_MASK;
    }
    if(IFS1bits.CNBIF & CNSTATBbits.CNSTATB8)
    {
        handle_interrupt(GPIO_PWM, GPIO_VALUE(B, 8));
        IFS1CLR = _IFS1_CNBIF_MASK;
        CNSTATBCLR = _CNSTATB_CNSTATB8_MASK;
    }
    if(IFS1bits.CNEIF & CNSTATEbits.CNSTATE5)
    {
        handle_interrupt(GPIO_CS, GPIO_VALUE(E, 5));
        IFS1CLR = _IFS1_CNEIF_MASK;
        CNSTATECLR = _CNSTATE_CNSTATE5_MASK;
    }
    if(IFS1bits.CNDIF & CNSTATDbits.CNSTATD6)
    {
        handle_interrupt(GPIO_RST, GPIO_VALUE(D, 6));
        IFS1CLR = _IFS1_CNDIF_MASK;
        CNSTATDCLR = _CNSTATD_CNSTATD6_MASK;
    }
}

int gpio_monitor_init(void)
{
    uint32_t i;
    for (i = 0; i < CALLBACK_COUNT; ++i)
        callbacks[i].callback = NULL;
    for (i = 0; i < TYPE_COUNT; ++i)
        callback_count_per_pin[i] = 0;

    current_callback_ID = 0;

    IPC8SET = (INTERRUPT_PRIORITY << _IPC8_CNIP_POSITION) | (INTERRUPT_SUBPRIORITY << _IPC8_CNIS_POSITION);

    return 0;
}

int gpio_monitor_add_callback(uint8_t gpio_pin, uint8_t event_mask, void(*callback)(uint8_t))
{
    uint32_t index, type_pin;

    if (callback == NULL)
    {
        fprintf(stderr, "gpio_monitor: Callback cannot be null\n");
        return -1;
    }

    if (gpio_init(gpio_pin) < 0
    ||  gpio_set_direction(gpio_pin, GPIO_INPUT))
    {
        fprintf(stderr, "gpio_monitor: failed to init GPIO\n");
        return -1;
    }

    /* Find an empty slot in callbacks array */
    for (index = 0; index < CALLBACK_COUNT; ++index) {
        if (callbacks[index].callback == NULL)
            break;
    }
    if (index == CALLBACK_COUNT) {
        fprintf(stderr, "gpio_monitor: Reached maximum number of callbacks (limit: %d)\n", CALLBACK_COUNT);
        return -1;
    }

    callbacks[index].callback_ID = current_callback_ID;
    ++current_callback_ID;
    callbacks[index].gpio_pin = gpio_pin;
    callbacks[index].event_mask = event_mask;
    callbacks[index].callback = callback;

    /* Update callback_count_per_pin and enable interrupt if needed */
    type_pin = convert_gpio_pin(gpio_pin);
    ++callback_count_per_pin[type_pin];
    if (callback_count_per_pin[type_pin] == 1)
        enable_interrupt(gpio_pin);

    return callbacks[index].callback_ID;
}

int gpio_monitor_remove_callback(int callback_ID)
{
    uint32_t index, ID, type_pin;

    if(callback_ID < 0)
    {
        fprintf(stderr, "gpio_monitor: Invalid callback id");
        return -1;
    }

    ID = (uint32_t)callback_ID;

    for (index = 0; index < CALLBACK_COUNT; ++index) {
        if (callbacks[index].callback_ID == ID)
            break;
    }
    if (index == CALLBACK_COUNT) {
        fprintf(stderr, "gpio_monitor: Could not remove callback %u.\n", ID);
        return -1;
    }

    callbacks[index].callback = NULL;

    /* Update callback_count_per_pin and disable interrupt if needed */
    type_pin = convert_gpio_pin(callbacks[index].gpio_pin);
    --callback_count_per_pin[type_pin];
    if (callback_count_per_pin[type_pin] == 0)
        disable_interrupt(callbacks[index].gpio_pin);

    return 0;
}

int gpio_monitor_release(void)
{
    uint32_t i;

    IPC8CLR = (INTERRUPT_PRIORITY << _IPC8_CNIP_POSITION) | (INTERRUPT_SUBPRIORITY << _IPC8_CNIS_POSITION);

    for (i = 0; i < CALLBACK_COUNT; ++i)
        callbacks[i].callback = NULL;
    for (i = 0; i < TYPE_COUNT; ++i) {
        if (callback_count_per_pin[i] != 0) {
            uint8_t gpio_pin = 0;
            gpio_get_pin(MIKROBUS_1, i, &gpio_pin);
            disable_interrupt(gpio_pin);
        }
        callback_count_per_pin[i] = 0;
    }

    return 0;
}

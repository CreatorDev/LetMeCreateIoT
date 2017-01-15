#include <letmecreate/core/gpio_monitor.h>

#include <stdio.h>
#include <contiki.h>
#include <letmecreate/core/gpio.h>

#include "button-sensor.h"

#define INTERRUPT_PRIORITY 6
#define INTERRUPT_SUBPRIORITY 0

struct interrupt_callback
{
    uint8_t event_mask;
    void (*callback)(uint8_t);
};

enum callback_index
{
    AN_CALLBACK,
    PWM_CALLBACK,
    INT_CALLBACK,
    RST_CALLBACK,
    CS_CALLBACK,
    CALLBACK_COUNT
};

static struct interrupt_callback callbacks[CALLBACK_COUNT];

ISR(_CHANGE_NOTICE_VECTOR) {
    if(BUTTON1_CHECK_IRQ())
    {
        button1_isr();
    }
    else if(BUTTON2_CHECK_IRQ())
    {
        button2_isr();
    }

    int callback_id = -1;
    int value = -1;
    if(IFS1bits.CNGIF & CNSTATGbits.CNSTATG9 && callbacks[AN_CALLBACK].callback)
    {
        callback_id = AN_CALLBACK;
        value = GPIO_VALUE(G, 9);
        (void)PORTG;
        IFS1CLR = _IFS1_CNGIF_MASK;
        CNSTATGCLR = _CNSTATG_CNSTATG9_MASK;
    }
    if(IFS1bits.CNDIF & CNSTATDbits.CNSTATD0 && callbacks[INT_CALLBACK].callback)
    {
        callback_id = INT_CALLBACK;
        value = GPIO_VALUE(D, 0);
        (void)PORTD;
        IFS1CLR = _IFS1_CNDIF_MASK;
        CNSTATDCLR = _CNSTATD_CNSTATD0_MASK;
    }
    if(IFS1bits.CNFIF & CNSTATFbits.CNSTATF3 && callbacks[PWM_CALLBACK].callback)
    {
        callback_id = PWM_CALLBACK;
        value = GPIO_VALUE(F, 3);
        (void)PORTF;
        IFS1CLR = _IFS1_CNFIF_MASK;
        CNSTATFCLR = _CNSTATF_CNSTATF3_MASK;
    }
    if(IFS1bits.CNEIF & CNSTATEbits.CNSTATE5 && callbacks[CS_CALLBACK].callback)
    {
        callback_id = CS_CALLBACK;
        value = GPIO_VALUE(E, 5);
        (void)PORTE;
        IFS1CLR = _IFS1_CNEIF_MASK;
        CNSTATECLR = _CNSTATE_CNSTATE5_MASK;
    }
    if(IFS1bits.CNDIF & CNSTATDbits.CNSTATD6 && callbacks[RST_CALLBACK].callback)
    {
        callback_id = RST_CALLBACK;
        value = GPIO_VALUE(D, 6);
        (void)PORTD;
        IFS1CLR = _IFS1_CNDIF_MASK;
        CNSTATDCLR = _CNSTATD_CNSTATD6_MASK;
    }

    if(callback_id != -1 && value != -1)
    {
        struct interrupt_callback * call = &callbacks[callback_id];

        int event_type;
        if(value == 0)
            event_type = GPIO_FALLING;
        else
            event_type = GPIO_RAISING;
        if(call->event_mask & event_type)
            call->callback(event_type);
    }

}

int gpio_monitor_init(void)
{
    IPC8SET = (INTERRUPT_PRIORITY << _IPC8_CNIP_POSITION) | (INTERRUPT_SUBPRIORITY << _IPC8_CNIS_POSITION);

    return 0;
}

int gpio_monitor_add_callback(uint8_t gpio_pin, uint8_t event_mask, void(*callback)(uint8_t))
{
    if(callback == NULL)
    {
        fprintf(stderr, "gpio_monitor: Callback cannot be null\n");
        return -1;
    }

    if(gpio_init(gpio_pin) < 0 ||
        gpio_set_direction(gpio_pin, GPIO_INPUT))
    {
        fprintf(stderr, "gpio_monitor: failed to init GPIO\n");
        return -1;
    }

    int callback_id;
    switch(gpio_pin) {
        case GPIO_AN:
        CNCONGSET = _CNCONG_ON_MASK;
        IEC1SET = _IEC1_CNGIE_MASK;
        IFS1CLR = _IFS1_CNGIF_MASK;
        (void)PORTG;
        CNENGSET = _CNENG_CNIEG9_MASK;
        callback_id = AN_CALLBACK;
        break;
        case GPIO_PWM:
        CNCONFSET = _CNCONF_ON_MASK;
        IEC1SET = _IEC1_CNFIE_MASK;
        IFS1CLR = _IFS1_CNFIF_MASK;
        (void)PORTF;
        CNENFSET = _CNENF_CNIEF3_MASK;
        callback_id = PWM_CALLBACK;
        break;
        case GPIO_INT:
        CNCONDSET = _CNCOND_ON_MASK;
        IEC1SET = _IEC1_CNDIE_MASK;
        IFS1CLR = _IFS1_CNDIF_MASK;
        (void)PORTD;
        CNENDSET = _CNEND_CNIED0_MASK;
        callback_id = INT_CALLBACK;
        break;
        case GPIO_RST:
        CNCONDSET = _CNCOND_ON_MASK;
        IEC1SET = _IEC1_CNDIE_MASK;
        IFS1CLR = _IFS1_CNDIF_MASK;
        (void)PORTD;
        CNENDSET = _CNEND_CNIED6_MASK;
        callback_id = RST_CALLBACK;
        break;
        case GPIO_CS:
        CNCONESET = _CNCONE_ON_MASK;
        IEC1SET = _IEC1_CNEIE_MASK;
        IFS1CLR = _IFS1_CNEIF_MASK;
        (void)PORTE;
        CNENESET = _CNENE_CNIEE5_MASK;
        callback_id = CS_CALLBACK;
        break;
        default:
        fprintf(stderr, "gpio_monitor: Invalid pin");
        return -1;
    }

    callbacks[callback_id].event_mask = event_mask;
    callbacks[callback_id].callback = callback;
    return callback_id;
}

int gpio_monitor_remove_callback(int callback_ID)
{
    if(callback_ID < 0
        || callback_ID >= CALLBACK_COUNT
        || callbacks[callback_ID].callback == NULL)
    {
        fprintf(stderr, "gpio_monitor: Invalid callback id");
        return -1;
    }

    switch(callback_ID) {
        case AN_CALLBACK:
        CNCONGCLR = _CNCONG_ON_MASK;
        IEC1CLR = _IEC1_CNGIE_MASK;
        IFS1CLR = _IFS1_CNGIF_MASK;
        (void)PORTG;
        CNENGCLR = _CNENG_CNIEG9_MASK;
        break;
        case PWM_CALLBACK:
        CNCONFCLR = _CNCONF_ON_MASK;
        IEC1CLR = _IEC1_CNFIE_MASK;
        IFS1CLR = _IFS1_CNFIF_MASK;
        (void)PORTF;
        CNENFCLR = _CNENF_CNIEF3_MASK;
        break;
        case INT_CALLBACK:
        CNCONDCLR = _CNCOND_ON_MASK;
        IEC1CLR = _IEC1_CNDIE_MASK;
        IFS1CLR = _IFS1_CNDIF_MASK;
        (void)PORTD;
        CNENDCLR = _CNEND_CNIED0_MASK;
        break;
        case RST_CALLBACK:
        CNCONDCLR = _CNCOND_ON_MASK;
        IEC1CLR = _IEC1_CNDIE_MASK;
        IFS1CLR = _IFS1_CNDIF_MASK;
        (void)PORTD;
        CNENDCLR = _CNEND_CNIED6_MASK;
        break;
        case CS_CALLBACK:
        CNCONECLR = _CNCONE_ON_MASK;
        IEC1CLR = _IEC1_CNEIE_MASK;
        IFS1CLR = _IFS1_CNEIF_MASK;
        (void)PORTE;
        CNENECLR = _CNENE_CNIEE5_MASK;
        break;
        default:
        fprintf(stderr, "gpio_monitor: Invalid callback id\n");
        return -1;
    }

    callbacks[callback_ID].callback = NULL;

    return 0;
}



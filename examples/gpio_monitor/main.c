#include <stdio.h>
#include <contiki.h>

#include "dev/leds.h"

#include "letmecreate/core/debug.h"
#include "letmecreate/core/common.h"
#include "letmecreate/core/gpio_monitor.h"

void monitor_callback_an(uint8_t event)
{
    printf("Callback called for AN\n");
    leds_toggle(LED1);
}

void monitor_callback_int(uint8_t event)
{
    printf("Callback called for INT\n");
    leds_toggle(LED2);
}

void monitor_callback_pwm(uint8_t event)
{
    printf("Callback called for PWM\n");
    leds_toggle(LED1);
}

void monitor_callback_rst(uint8_t event)
{
    printf("Callback called for RST\n");
    leds_toggle(LED2);
}

void monitor_callback_cs(uint8_t event)
{
    printf("Callback called for CS\n");
    leds_toggle(LED1);
}


PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();
    INIT_NETWORK_DEBUG();
    {
        PRINTF("=====Start=====\n");

        gpio_monitor_init();
        gpio_monitor_add_callback(GPIO_AN, GPIO_RAISING, monitor_callback_an);
        gpio_monitor_add_callback(GPIO_INT, GPIO_RAISING, monitor_callback_int);
        gpio_monitor_add_callback(GPIO_PWM, GPIO_EDGE, monitor_callback_pwm);
        gpio_monitor_add_callback(GPIO_RST, GPIO_FALLING, monitor_callback_rst);
        gpio_monitor_add_callback(GPIO_CS, GPIO_FALLING, monitor_callback_cs);
        while(1)
        {

        }
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

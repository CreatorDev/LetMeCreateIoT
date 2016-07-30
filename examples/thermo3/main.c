#include <stdio.h>

#include <contiki.h>

#include <sys/clock.h>

#include <letmecreate/core/i2c.h>
#include <letmecreate/click/thermo3.h>

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();

    {
        static struct etimer et;
        static float temperature = 0.0f;

        printf("=====Start=====\n");

        i2c_init();

        thermo3_click_enable(0);

        while(1)
        {
            etimer_set(&et, CLOCK_SECOND);
            PROCESS_WAIT_EVENT();

            thermo3_click_get_temperature(&temperature);

            // Printf for float doesn't work on Contiki, however we can
            // still use floats in general
            printf("Current temperature: %i\n", (uint32_t)temperature);

        }

        thermo3_click_disable();
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

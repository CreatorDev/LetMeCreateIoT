#include <stdio.h>

#include <contiki.h>

#include <sys/clock.h>

#include "letmecreate/click/color2.h"
#include "letmecreate/core/debug.h"

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();
    INIT_NETWORK_DEBUG();
    {
        static uint16_t r, g, b;

        PRINTF("=====Start=====\n");
        i2c_init();
        color2_click_enable();

        color2_click_get_color(&r, &g, &b);
        PRINTF("red: %u\n", r);
        PRINTF("green: %u\n", g);
        PRINTF("blue: %u\n\n", b);

        color2_click_disable();
        i2c_release();
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

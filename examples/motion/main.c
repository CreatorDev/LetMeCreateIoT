#include <stdio.h>

#include <contiki.h>

#include <sys/clock.h>

#include "dev/leds.h"

#include "letmecreate/click/motion.h"
#include "letmecreate/core/interrupts.h"
#include "letmecreate/core/common.h"

#include "letmecreate/core/debug.h"

void motion_callback(uint8_t event)
{
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

        motion_click_enable(MIKROBUS_1);

        motion_click_attach_callback(MIKROBUS_1, motion_callback);
        while(1)
        {
        }

        motion_click_disable(MIKROBUS_1);
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

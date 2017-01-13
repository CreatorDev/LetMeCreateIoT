#include <stdio.h>

#include <contiki.h>

#include <sys/clock.h>

#include "letmecreate/core/power.h"

#include "letmecreate/core/debug.h"

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();
    INIT_NETWORK_DEBUG();
    {
        PRINTF("=====Start=====\n");

        // Disable all except CA8210 SPI. Radio will be disabled as well, add PERIPHERAL_RADIO
        // to enable it
        power_disable_peripherals(PERIPHERAL_ALL & ~PERIPHERAL_SPI2);

        // Go to sleep and loop
        while(1)
        {
            power_start_sleep();
            // Won't print over network since RADIO is disabled, enable PERIPHERAL_RADIO above
            // if you want to use 6lowpan debug
            PRINTF("Woke up from sleep\n");
        }
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

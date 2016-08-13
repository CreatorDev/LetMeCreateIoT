#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include "dev/leds.h"
#include "letmecreate/core/debug.h"
#include "letmecreate/core/network.h"

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();
    INIT_NETWORK_DEBUG();
    {
        // Due to the way Contiki protothreads work this needs to be static,
        // otherwise the data will be lost when switching to a different thread
        static int i = 0;
        PRINTF("===START===\n");

#ifdef DEBUG_IP
        leds_on(LED1);
#endif

        PRINTF("Starting loop\n");

        while(1)
        {
            PRINTF("Looping like crazy %i\n", i);
            i++;
        }

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

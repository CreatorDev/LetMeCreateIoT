#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include "dev/leds.h"
#include "letmecreate/core/common.h"
#include "letmecreate/core/debug.h"
#include "letmecreate/core/adc.h"

#include <p32xxxx.h>

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
        static float voltage = 0.0f;
        static uint16_t major = 0;
        static uint16_t minor = 0;
        PRINTF("===START===\n");

        PRINTF("Starting loop\n");

        while(1)
        {
            adc_get_value(MIKROBUS_1, &voltage);

            major = (int)voltage;
            minor = (int)((voltage - (float)major) * 100.0f);
            PRINTF("Voltage: %i.%02i\n", major, minor);
        }

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

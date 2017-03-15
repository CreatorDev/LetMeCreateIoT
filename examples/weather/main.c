#include <stdio.h>
#include <contiki.h>
#include <contiki-net.h>

#include "letmecreate/core/debug.h"
#include "letmecreate/core/i2c.h"
#include "letmecreate/click/weather.h"

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
        static struct etimer et;
        static double temperature = 0.0;
        static double pressure = 0.0;
        static double humidity = 0.0;

        PRINTF("===START===\n");

        i2c_init();
        weather_click_enable();

        while (1)
        {
            weather_click_read_measurements(&temperature, &pressure, &humidity);
            PRINTF("Temperature: %i Pressure: %i Humidity: %i\n",
                   (int)temperature, (int)pressure, (int)humidity);

            etimer_set(&et, CLOCK_SECOND);
            PROCESS_WAIT_EVENT();
        }

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

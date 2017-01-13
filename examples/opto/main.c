#include <stdio.h>
#include <contiki.h>

#include "dev/leds.h"

#include "letmecreate/click/opto.h"
#include "letmecreate/core/debug.h"
#include "letmecreate/core/common.h"

void opto_callback(uint8_t event)
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

        opto_click_attach_callback(MIKROBUS_1, OPTO_CLICK_CHANNEL_4, opto_callback);
        while(1)
        {
            uint8_t state;
            opto_click_read_channel(MIKROBUS_1, OPTO_CLICK_CHANNEL_4, &state);
            PRINTF("Opto state: %i\n");
        }
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

/* Based on LetMeCreate's Bargraph example */

#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include <letmecreate/letmecreate.h>

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();
    INIT_NETWORK_DEBUG();
    {
        static uint16_t value;
        PRINTF("=====Start=====\n");

        spi_init();

        /* Set LED's to max intensity */
        bargraph_click_set_intensity(MIKROBUS_1, 100.f);

        /* Gradually turn on each LED. */
        for (; value < 0x400; value <<= 1, value |= 1) {
            PRINTF("Changing bars \n");
            bargraph_click_set_value(value);
            sleep_ms(200);
        }

        spi_release();
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

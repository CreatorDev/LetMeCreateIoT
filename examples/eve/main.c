/**
 * Ported from the original example written by Francois Berder as part
 * of the LMC library
 * This example shows some basic features of the EVE Click wrapper of the
 * LetMeCreate library.
 *
 * It displays a green screen for three seconds, then displays some text for
 * another three seconds.
 *
 * Before running this program:
 *   - the EVE Click must be inserted in Mikrobus 1
 *   - a WQVGA screen must be connected to the EVE Click.
 */
#include <stdio.h>
#include <contiki.h>

#include "letmecreate/core/spi.h"
#include "letmecreate/core/common.h"
#include "letmecreate/core/debug.h"
#include "letmecreate/click/eve.h"
PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();
    INIT_NETWORK_DEBUG();
    {
        static struct etimer et;
        PRINTF("=====Start=====\n");

        spi_init(0);

        if (eve_click_enable(MIKROBUS_1) < 0)
            return -1;

        /* Display a green screen for 3 seconds */
        eve_click_clear(0, 255, 0);
        eve_click_display();

        etimer_set(&et, CLOCK_SECOND);
        PROCESS_WAIT_EVENT();

        /* Display some text */
        eve_click_clear(0, 0, 0);   /* black screen */
        eve_click_draw(FT800_TEXT,
                       240,                 /* x */
                       136,                 /* y */
                       31,                  /* font */
                       FT800_OPT_CENTER,    /* options */
                        "Hello World !");
        eve_click_display();

        etimer_set(&et, CLOCK_SECOND);
        PROCESS_WAIT_EVENT();

        eve_click_disable(MIKROBUS_1);
        spi_release();

    }

    PROCESS_END();
}

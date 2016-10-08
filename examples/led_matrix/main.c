#include <stdio.h>

#include <contiki.h>

#include <sys/clock.h>

#include "letmecreate/core/spi.h"
#include "letmecreate/click/led_matrix.h"

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();

    {
        static struct etimer et;
        static int i;

        printf("=====Start=====\n");

        spi_init(3);

        led_matrix_click_enable();
        led_matrix_click_set_intensity(3);

        for(i = 0; i <= 99; i++)
        {
            printf("Displaying %i\n", i);
            led_matrix_click_display_number(i);

            etimer_set(&et, CLOCK_SECOND);
            PROCESS_WAIT_EVENT();
        }

        led_matrix_click_disable();
        spi_release();
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

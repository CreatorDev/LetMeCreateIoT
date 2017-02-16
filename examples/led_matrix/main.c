#include <stdio.h>

#include <contiki.h>

#include <sys/clock.h>

#include "letmecreate/core/common.h"
#include "letmecreate/core/spi.h"
#include "letmecreate/click/led_matrix.h"

#include "letmecreate/core/debug.h"

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();
    INIT_NETWORK_DEBUG();
    {
        static struct etimer et;
        static int i;

        PRINTF("=====Start=====\n");

        if(spi_init() < 0 ||
            spi_set_mode(MIKROBUS_1, SPI_MODE_3) < 0)
        {
            PRINTF("SPI init failed\n");
            return 1;
        }

        PRINTF("SPI init passed\n");

        if(led_matrix_click_enable() < 0 ||
           led_matrix_click_set_intensity(3) < 0)
        {
            PRINTF("LED matrix init failed\n");
            return 1;
        }

        PRINTF("Display numbers\n");
        for(i = 0; i <= 99; i++)
        {
            PRINTF("Displaying %i\n", i);
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

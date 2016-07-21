#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include <sys/clock.h>

#include <string.h>

#include <lmc/click/accel.h>

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();

    {
        int i;
        printf("=====Start=====\n");

        accel_click_enable();

        while(1)
        {
            for(i = 0; i < 2; i++)
                    clock_delay_usec(65000);

            float x, y, z;
            accel_click_get_measure(&x, &y, &z);

            printf("%f %f %f\n", x, y, z);

        }

        accel_click_disable();
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

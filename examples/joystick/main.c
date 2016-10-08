#include <stdio.h>

#include <contiki.h>

#include <sys/clock.h>

#include "letmecreate/core/i2c.h"
#include "letmecreate/click/joystick.h"

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();

    {
        static int8_t x, y;

        printf("=====Start=====\n");

        i2c_init();
        while(1)
        {
            joystick_click_get_x(&x);
            joystick_click_get_y(&y);
            printf("Position: %i %i\n", x, y);
        }
        i2c_release();
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

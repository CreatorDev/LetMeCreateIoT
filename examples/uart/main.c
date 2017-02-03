#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include "letmecreate/core/debug.h"
#include "letmecreate/core/uart.h"

#include <p32xxxx.h>

#define DATA_LENGTH 8

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
        static unsigned char data[DATA_LENGTH];
        static int i = 0;
        uart_init();

        PRINTF("===START===\n");

        while(1)
        {
            PRINTF("Waiting to receive...\n");
            /* Accepts 8 bytes and pings those 8 bytes back to the sender */
            uart_receive(data, DATA_LENGTH);
            PRINTF("Sending: ");
            for(i = 0; i < DATA_LENGTH; i++)
            {
                PRINTF("%i", data[i]);
            }
            PRINTF("\n");
            uart_send(data, DATA_LENGTH);
        }

        uart_release();
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

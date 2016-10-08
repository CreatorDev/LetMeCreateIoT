#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include "letmecreate/core/i2c.h"
#include "letmecreate/core/network.h"
#include "letmecreate/click/thermo3.h"

#include "letmecreate/core/debug.h"

#define SERVER_IP_ADDR "fe80::e01a:69ff:fe6c:4a97"

#define SERVER_PORT 3000
#define CLIENT_PORT 3001

#define BUFFER_SIZE 64

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
        static struct uip_udp_conn * conn;
        static float temperature;
        static uint8_t network_data[2];

        PRINTF("===START===\n");

        i2c_init();
        conn = udp_new_connection(CLIENT_PORT, SERVER_PORT, SERVER_IP_ADDR);

        thermo3_click_enable(0);
        while(1)
        {
            etimer_set(&et, CLOCK_SECOND);
            PROCESS_WAIT_EVENT();

            thermo3_click_get_temperature(&temperature);

            // We don't want to send floats over, so the simples solution is to send
            // the integer part plus the first two decimal point digits as integers,
            // then we decode them on the server
            network_data[0] = (uint8_t)temperature;
            network_data[1] = (uint8_t)((temperature - (float)network_data[0]) * 100.0);

            PRINTF("Sending temperature %i.%i\n", network_data[0], network_data[1]);

            udp_packet_send(conn, network_data, 2);
            PROCESS_WAIT_UDP_SENT();
        }

        thermo3_click_disable();
        i2c_release();
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

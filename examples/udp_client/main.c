#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include "letmecreate/core/network.h"

#include "letmecreate/core/debug.h"

#define SERVER_IP_ADDR "fe80:0000:0000:0000:28e9:3285:421c:bc82"

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
        static struct uip_udp_conn * conn;
        static char buffer[BUFFER_SIZE];
        static int i = 0;
        PRINTF("===START===\n");

        conn = udp_new_connection(CLIENT_PORT, SERVER_PORT, SERVER_IP_ADDR);

        while(1)
        {
            i++;
            sprintf(buffer, "Hello number %i from client", i);

            PRINTF("Sending data: %s\n", buffer);

            udp_packet_send(conn, buffer, strlen(buffer));
            PROCESS_WAIT_UDP_SENT();
        }

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

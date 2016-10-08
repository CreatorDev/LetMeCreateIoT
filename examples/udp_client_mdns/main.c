#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include "dev/leds.h"
#include "letmecreate/core/network.h"

// Used for PRINT6ADDR function
#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#include "letmecreate/core/debug.h"

#define SERVER_PORT 3000
#define CLIENT_PORT 3001

#define BUFFER_SIZE 64

#define RESOLV_NAME "contiki-server.local"

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
        static uip_ipaddr_t * resolved_addr = NULL;
        static struct uip_udp_conn * conn;
        static char buffer[BUFFER_SIZE];
        static int i = 0;

        PRINTF("===START===\n");

        resolv_query(RESOLV_NAME);
        PROCESS_WAIT_EVENT_UNTIL(ev == resolv_event_found);

        resolv_lookup(RESOLV_NAME, &resolved_addr);

        if(resolved_addr == NULL)
        {
            leds_on(LED1);
            return 1;
        }

        conn = udp_new(resolved_addr, UIP_HTONS(SERVER_PORT), NULL);
        udp_bind(conn, UIP_HTONS(CLIENT_PORT));

        PRINTF("IP: ");
        PRINT6ADDR(resolved_addr);
        PRINTF("\n");
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

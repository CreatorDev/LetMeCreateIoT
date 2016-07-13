#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>


#define SERVER_IP_ADDR "fe80::e01a:69ff:fe6c:4a97"

#define SERVER_PORT 5000

#define BUFFER_SIZE 64

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();

    {
        // Due to the way Contiki protothreads work this needs to be static,
        // otherwise the data will be lost when switching to a different thread
        static struct etimer et;
        static struct uip_udp_conn * conn;
        static char buffer[BUFFER_SIZE];
        static int i = 0;
        printf("===START===\n");

        // First convert the IP address into a structure
        uip_ipaddr_t addr;
        uiplib_ipaddrconv(SERVER_IP_ADDR, &addr);

        // Create a new udp connection
        conn = udp_new(&addr, UIP_HTONS(SERVER_PORT), NULL);

        // Bind the connection locally to a random port
        udp_bind(conn, UIP_HTONS(0));

        while(1)
        {
            i++;
            sprintf(buffer, "Hello number %i from client", i);
            // We need this timeout timer for the event wait. We need to
            // wait between each packet for UDP to work.
            etimer_set(&et, CLOCK_SECOND/5);

            printf("Sending data: %s\n", buffer);
            uip_udp_packet_send(conn, buffer, strlen(buffer));

            // Halt the process until the timer event finishes
            PROCESS_WAIT_EVENT();
        }

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

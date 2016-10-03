#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include "letmecreate/core/network.h"

// Used for PRINT6ADDR function
#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"


#define SERVER_PORT 3000
#define CLIENT_PORT 3001
#define SERVER_IP_ADDR "fe80:0000:0000:0000:28e9:3285:421c:bc82"
#define BUFFER_SIZE 4096

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();

    {
        // Due to the way Contiki protothreads work this needs to be static,
        // otherwise the data will be lost when switching to a different thread
        static struct uip_udp_conn * conn;
        static char network_data[BUFFER_SIZE];
        static int length;
        static struct uip_ip_hdr metadata;

        printf("===START===\n");

        // Making our IP address constant to match the other example
        if(ipv6_add_address(SERVER_IP_ADDR, NULL, 0) < 0)
        {
            printf("Failed to set IPV6 address\n");
            return 1;
        }

        // Bind a new connection, needs to be called on both sides
        conn = udp_new_connection(SERVER_PORT, CLIENT_PORT, NULL);

        while(1)
        {
            printf("Waiting for data...\n");

            PROCESS_WAIT_UDP_RECEIVED();
            length = udp_packet_receive(network_data, BUFFER_SIZE, &metadata);

            network_data[length] = '\0';
            printf("Received %s from:\n", network_data);
            PRINT6ADDR(&metadata.srcipaddr);
            printf("\n=====================\n");

        }

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

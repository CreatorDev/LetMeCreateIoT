#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include "lmc/core/network.h"

// Used for PRINT6ADDR function
#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

// Macro taken from Contiki examples, it lets us easily access the packet
// buffer to restore eg. sender's IP address
#define UIP_IP_BUF ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])

#define SERVER_PORT 3000
#define CLIENT_PORT 3001

#define SERVER_IP_ADDR "fe80:0000:0000:0000:28e9:3285:421c:bc82"

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
        static char * network_data;
        printf("===START===\n");

        // Making our IP address constant to match the other example
        if(set_ipv6_address(SERVER_IP_ADDR, NULL))
        {
            printf("Failed to set IPV6 address\n");
            return 1;
        }

        // Below steps can be avoided by using the udp_new_connection function from core/network.h
        // Create a new udp connection
        conn = udp_new(NULL, UIP_HTONS(CLIENT_PORT), NULL);

        // Bind the connection locally to a port. For the connection to work both server & client
        // need to call their respective ports in udp_new & udp_bind
        udp_bind(conn, UIP_HTONS(SERVER_PORT));

        while(1)
        {
            printf("Waiting for data...\n");
            // Give up the process until some network event pops up
            PROCESS_YIELD();
            if(ev == tcpip_event)
            {
                printf("Process woken up\n");
                if(uip_newdata())
                {
                    // Get the data as null-character terminated text
                    network_data = (char*)uip_appdata;
                    network_data[uip_datalen()] = "\0";
                    printf("Received %s from:\n", network_data);
                    PRINT6ADDR(&UIP_IP_BUF->srcipaddr);
                    printf("\n=====================\n");
                }
            }
        }

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

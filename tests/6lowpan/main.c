#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include "dev/leds.h"

#include "letmecreate/core/network.h"

#define SERVER_IP_ADDR "2001:1418:100::1"

#define SERVER_PORT 3000
#define CLIENT_PORT 3001

#define BUFFER_SIZE 64

PROCESS(main_process, "Main process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();

    {
        static struct uip_udp_conn * conn;
        static char buffer[BUFFER_SIZE];
        printf("===START===\n");

        ipv6_add_default_route(SERVER_IP_ADDR, 0);

        leds_on(LED1);
        leds_on(LED2);

        printf("Creating connection...\n");

        conn = udp_new_connection(CLIENT_PORT, SERVER_PORT, SERVER_IP_ADDR);
        PROCESS_WAIT_UDP_CONNECTED();

        printf("Sending hello\n");

        strncpy(buffer, "Hello", strlen("Hello"));

        printf("Sending data: %s\n", buffer);
        udp_packet_send(conn, buffer, strlen(buffer));
        PROCESS_WAIT_UDP_SENT();

        printf("Waiting for receive\n");

        PROCESS_WAIT_UDP_RECEIVED();
        udp_packet_receive(buffer, sizeof(buffer), NULL);

        printf("Received: %s\n", buffer);

        if(strcmp("Hello bounced", buffer) == 0)
        {
            leds_off(LED1);
        }
        else
        {
            leds_off(LED2);
        }

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

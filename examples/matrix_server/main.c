#include <stdio.h>

#include <contiki.h>
#include <contiki-net.h>

#include "dev/leds.h"

#include "letmecreate/core/spi.h"
#include "letmecreate/core/network.h"
#include "letmecreate/click/led_matrix.h"

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
        printf("===START===\n");

        // Making our IP address constant to match the other example
        if(ipv6_add_address(SERVER_IP_ADDR, NULL, 0) < 0)
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

        spi_init(3);

        led_matrix_click_enable();
        led_matrix_click_display_number(0);

        while(1)
        {
            leds_toggle(LED1);
            printf("Waiting for data...\n");
            // Give up the process until some network event pops up
            PROCESS_YIELD();
            if(ev == tcpip_event)
            {
                leds_toggle(LED2);
                if(uip_newdata())
                {
                    int val = *((int*)uip_appdata);
                    led_matrix_click_display_number(val);
                }
            }
        }

        led_matrix_click_disable();
        spi_release();

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <contiki.h>
#include <contiki-net.h>
#include "dev/leds.h"

#include "letmecreate/core/network.h"
#include "letmecreate/core/debug.h"
#include "letmecreate/core/i2c.h"
#include "letmecreate/click/weather.h"

#define SERVER_IP_ADDR "fe80:0000:0000:0000:0019:f5ff:fe89:e0c"

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
        static struct etimer et;
        static double temperature = 0.0;
        static double pressure = 0.0;
        static double humidity = 0.0;

        // Due to the way Contiki protothreads work this needs to be static,
        // otherwise the data will be lost when switching to a different thread
        static struct uip_udp_conn * conn;
        static char buffer[BUFFER_SIZE];
        PRINTF("===START===\n");

        i2c_init();
        weather_click_enable();
        
        conn = udp_new_connection(CLIENT_PORT, SERVER_PORT, SERVER_IP_ADDR);

        while (1)
        {
            // Read and send sensor data every 10 seconds
            etimer_set(&et, 10*CLOCK_SECOND);
            PROCESS_WAIT_EVENT();

            leds_on(LED1);

            weather_click_read_measurements(&temperature, &pressure, &humidity);

            // Contiki printf() does not support float/double
            int itemp = (int)10*temperature;
            int ftemp = abs(itemp%10);
            int ntemp = itemp/10;
            
            int ipress = (int)pressure;
            int fpress = abs(ipress%100);
            int npress = ipress/100;
            
            int ihum = (int)10*humidity;
            int fhum = abs(ihum%10);
            int nhum = ihum/10;
            
            sprintf(buffer, "{\"temp\": %d.%d, \"press\": %d.%02d, \"hum\":%d.%d}", 
                ntemp, ftemp, npress, fpress, nhum, fhum);

            udp_packet_send(conn, buffer, strlen(buffer));
            PROCESS_WAIT_UDP_SENT();

            leds_off(LED1);
        }

    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/

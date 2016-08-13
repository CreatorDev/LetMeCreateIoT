#include <letmecreate/core/debug.h>

#include <stdio.h>
#include <string.h>

#include <contiki.h>
#include <contiki-net.h>

#include <letmecreate/core/network.h>


#define XSTR(s) #s
#define STR(s) XSTR(s)
#define DEBUG_BUFFER_SIZE 1024
#define DEBUG_PORT 9999

struct uip_udp_conn * debug_connection = NULL;
static uint32_t packet_index = 0;

void init_debug_connection()
{
#ifdef DEBUG_IP
    debug_connection = udp_new_connection(6000, DEBUG_PORT, STR(DEBUG_IP));
#endif
}

int network_printf(const char * format, ...)
{
    if(debug_connection == NULL)
        return -1;

    va_list list;
    va_start(list, format);
    char message[DEBUG_BUFFER_SIZE + sizeof(packet_index)];
    int ret = vsnprintf(message + sizeof(packet_index), DEBUG_BUFFER_SIZE, format, list);
    va_end(list);

    memcpy(message, &packet_index, sizeof(packet_index));
    vprintf(format, list);

    if(udp_packet_send(debug_connection, message,
        strlen(message + sizeof(packet_index)) + sizeof(packet_index)) < 0)
    {
        return -1;
    }
    packet_index++;

    return ret;
}

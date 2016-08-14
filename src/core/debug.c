#include <letmecreate/core/debug.h>

#include <stdio.h>
#include <string.h>

#include <contiki.h>
#include <contiki-net.h>

#include <letmecreate/core/network.h>


#define XSTR(s) #s
#define STR(s) XSTR(s)

struct uip_udp_conn * __debug_connection = NULL;
#define DEBUG_BUFFER_SIZE 1024

static uint32_t packet_index = 0;

void __init_debug_connection()
{
#ifdef DEBUG_IP
    __debug_connection = udp_new_connection(6000, DEBUG_PORT, STR(DEBUG_IP));
#endif
}

int __network_printf(const char * format, ...)
{
    if(__debug_connection == NULL)
        return -1;

    va_list list;
    va_start(list, format);
    char message[DEBUG_BUFFER_SIZE + sizeof(packet_index)];
    int ret = vsnprintf(message + sizeof(packet_index), DEBUG_BUFFER_SIZE, format, list);
    va_end(list);

    memcpy(message, &packet_index, sizeof(packet_index));
    vprintf(format, list);

    if(udp_packet_send(__debug_connection, message,
        strlen(message + sizeof(packet_index)) + sizeof(packet_index)) < 0)
    {
        return -1;
    }
    packet_index++;

    return ret;
}

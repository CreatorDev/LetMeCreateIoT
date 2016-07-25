#include <contiki.h>
#include <contiki-net.h>

#include <stdio.h>

#include "network.h"

uint8_t set_ipv6_address(const char * address, uip_ipaddr_t * addr)
{
    uip_ipaddr_t tmp_addr;

    if(!addr)
        addr = &tmp_addr;

    if(!uiplib_ipaddrconv(address, addr))
    {
        printf("Network: Failed to convert IP: %s\n", address);
        return 1;
    }

    if(!uip_ds6_addr_add(addr, 0, ADDR_MANUAL))
    {
        printf("Network: Failed to set IP: %s\n", address);
        return 1;
    }

    return 0;
}

struct uip_udp_conn * udp_new_connection(uint16_t local_port, uint16_t remote_port, const char * address)
{
    struct uip_udp_conn * conn = NULL;

    if(address)
    {
        uip_ipaddr_t addr;
        if(!uiplib_ipaddrconv(address, &addr))
        {
            printf("UDP: Failed to convert IP: %s\n", address);
            return NULL;
        }
        conn = udp_new(&addr, UIP_HTONS(remote_port), NULL);
    }
    else
    {
        conn = udp_new(NULL, UIP_HTONS(remote_port), NULL);
    }

    if(!conn)
    {
        printf("UDP: Failed to create connection\n");
        return NULL;
    }

    udp_bind(conn, UIP_HTONS(local_port));

    return conn;
}

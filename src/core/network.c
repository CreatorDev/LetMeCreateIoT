#include <contiki.h>
#include <contiki-net.h>

#include <stdio.h>

#include "network.h"

uint8_t network_set_ipv6_address(const char * address, uip_ipaddr_t * addr)
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



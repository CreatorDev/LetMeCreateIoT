#ifndef H_CORE_NETWORK
#define H_CORE_NETWORK

#include <stdint.h>

uint8_t network_set_ipv6_address(const char * address, uip_ipaddr_t * addr);

#endif

#ifndef H_CORE_NETWORK
#define H_CORE_NETWORK

#include <stdint.h>

struct uip_udp_conn;

uint8_t set_ipv6_address(const char * address, uip_ipaddr_t * addr);
struct uip_udp_conn * udp_new_connection(uint16_t local_port, uint16_t remote_port, const char * remote_address);
#endif

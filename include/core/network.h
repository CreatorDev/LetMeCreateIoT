/**
 * @file network.h
 * @author Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef H_CORE_NETWORK
#define H_CORE_NETWORK

#include <stdint.h>

struct uip_udp_conn;

/**
 * @brief Sets local IPv6 address
 *
 * @param[in] address String containing the address to be set.
 * @param[out] addr Pointer to an IP addr structure to be filled with parsed data. Optional
 * @return 0 if successful, otherwise -1.
 */
int set_ipv6_address(const char * address, uip_ipaddr_t * addr);

/**
 * @brief Creates a new UDP connection between two endpoints.
 *
 * Creates and returns a new UDP connection. For the communication to work the remote host
 * has to bind a socket to @p remote_port and connect itself remotely to @p local_port
 *
 * @param[in] local_port Port to bind to locally
 * @param[in] remote_port Remote port to connect to
 * @return Pointer to a new connection if successful, otherwise NULL
 */
struct uip_udp_conn * udp_new_connection(uint16_t local_port, uint16_t remote_port, const char * remote_address);




#endif

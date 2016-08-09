/**
 * @file network.h
 * @author Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef H_CORE_NETWORK
#define H_CORE_NETWORK

#include <stdint.h>
#include <contiki-net.h>

/** Halts the process until UDP data has been sent */
#define PROCESS_WAIT_UDP_SENT() PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event)

/** Halts the process until UDP data is ready to be received */
#define PROCESS_WAIT_UDP_RECEIVED() PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event && uip_newdata())

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

/**
 * @brief Sends data over a UDP connection.
 *
 * Uses the IP provided in the connection structure to send a buffer of data
 *
 * @param[in] connection UDP connection
 * @param[in] data Data buffer
 * @param[in] len Length of the data to be sent
 * @return 0 if successful, -1 if failed
 */
int udp_packet_send(struct uip_udp_conn * connection, const uint8_t * data, uint32_t len);

/**
 * @brief Sends data over a UDP connection to one specific client.
 *
 * Sends a packet of data to one client as specified by the address
 *
 * @param[in] connection UDP connection
 * @param[in] data Data buffer
 * @param[in] len Length of the data to be sent
 * @param[in] address String containing the client's IP address
 * @param[in] port Remote port to send to
 * @return 0 if successful, -1 if failed
 */
int udp_packet_sendto(struct uip_udp_conn * connection, const uint8_t * data, uint32_t len,
    const char * address, uint16_t port);

/**
 * @brief Receives a UDP packet.
 *
 * Checks for and receives data available in the UDP buffer
 *
 * @param[out] data Data buffer to write to
 * @param[in] len Length of the data buffer
 * @param[out] packet_data Optional, structure containing metadata for the packet
 * @return 0 if no data available, length of the received data if found, -1 if failed
 */
int udp_packet_receive(uint8_t * data, uint32_t len, struct uip_ip_hdr * packet_data);

#endif

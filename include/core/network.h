/**
 * @file network.h
 * @author Michal Tusnio, Julien D
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef H_CORE_NETWORK
#define H_CORE_NETWORK

#include <stdint.h>
#include <contiki-net.h>

/** Halts the process until UDP data has been sent */
#define PROCESS_WAIT_UDP_SENT() PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event)

/** Halts the process until TCP data has been sent */
#define PROCESS_WAIT_TCP_SENT() PROCESS_WAIT_UDP_SENT()

/** Halts the process until UDP data is ready to be received */
#define PROCESS_WAIT_UDP_RECEIVED() PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event && uip_newdata())

/** Halts the process until TCP data is ready to be received */
#define PROCESS_WAIT_TCP_RECEIVED() PROCESS_WAIT_UDP_RECEIVED()

/** Halts the process until the UDP connection has been made */
#define PROCESS_WAIT_UDP_CONNECTED() do {                               \
                   static struct etimer timer;                          \
                   etimer_set(&timer, CLOCK_SECOND);                    \
                   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));    \
                   } while(0)

/** Halts the process until the TCP connection has been made */
#define PROCESS_WAIT_TCP_CONNECTED() PROCESS_WAIT_UDP_CONNECTED()

/**
 * @brief Adds a local IPv6 address
 *
 * @param[in] address String containing the address to be set.
 * @param[out] addr Pointer to an IP addr structure to be filled with IP address data. Optional
 * @param[in] lifetime How long, in seconds, the address should last. 0 for infinitely
 * @return 0 if successfully added, otherwise -1.
 */
int ipv6_add_address(const char * address, uip_ipaddr_t * addr, unsigned long lifetime);

/**
 * @brief Adds an IPv6 default route
 *
 * @param[in] address String containing the address to be added.
 * @param[in] lifetime How long, in seconds, the default address should last. 0 for infinitely
 * @return 0 if successfully added, otherwise -1.
 */
int ipv6_add_default_route(const char * address, unsigned long lifetime);

/**
 * @brief Removes an IPv6 default route
 *
 * @param[in] address String containing the address to be removed.
 * @return 0 if found and removed, otherwise -1.
 */
int ipv6_remove_default_route(const char * address);

/**
 * @brief Creates a new UDP connection between two endpoints.
 *
 * Creates and returns a new UDP connection. For the communication to work the remote host
 * has to bind a socket to @p remote_port and connect itself remotely to @p local_port
 *
 * @param[in] local_port Port to bind to locally
 * @param[in] remote_port Remote port to connect to
 * @param[in] remote_address Optional, remote address to connect to with this connection. NULL if none
 * @return Pointer to a new connection if successful, otherwise NULL
 */
struct uip_udp_conn * udp_new_connection(uint16_t local_port, uint16_t remote_port, const char * remote_address);

/**
 * @brief Sends data over a UDP connection.
 *
 * Uses the IP provided in the connection structure to send a buffer of data.
 * To make this call blocking until data has been sent call #PROCESS_WAIT_UDP_SENT after this function
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
 * To make this call blocking until data has been sent call #PROCESS_WAIT_UDP_SENT after this function
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
 * In order to block until there's any data available call #PROCESS_WAIT_UDP_RECEIVED before this function
 *
 * @param[out] data Data buffer to write to
 * @param[in] len Length of the data buffer
 * @param[out] packet_data Optional, structure containing metadata for the packet
 * @return 0 if no data available, length of the received data if found, -1 if failed
 */
int udp_packet_receive(uint8_t * data, uint32_t len, struct uip_ip_hdr * packet_data);

/**
 * @brief Creates a new TCP connection between two endpoints.
 *
 * Creates and returns a new TCP connection. For the communication to work the remote host
 * has to bind a socket to @p remote_port
 *
 * @param[in] remote_port Remote port to connect to
 * @param[in] remote_address, remote address to connect to with this connection.
 * @return Pointer to a new connection if successful, otherwise NULL
 */
struct uip_conn * tcp_new_connection(uint16_t remote_port, const char * remote_address);

/**
 * @brief Sends data over a TCP connection.
 *
 * Uses the IP provided in the connection structure to send a buffer of data.
 * To make this call blocking until data has been sent call #PROCESS_WAIT_TCP_SENT after this function
 *
 * @param[in] connection TCP connection
 * @param[in] data Data buffer
 * @param[in] len Length of the data to be sent
 * @return 0 if successful, -1 if failed
 */
int tcp_packet_send(struct uip_conn * connection, const uint8_t * data, uint32_t len);


/**
 * @brief Receives a TCP packet.
 *
 * Checks for and receives data available in the TCP buffer
 * In order to block until there's any data available call #PROCESS_WAIT_TCP_RECEIVED before this function
 *
 * @param[out] data Data buffer to write to
 * @param[in] len Length of the data buffer
 * @param[out] packet_data Optional, structure containing metadata for the packet
 * @return 0 if no data available, length of the received data if found, -1 if failed
 */
int tcp_packet_receive(uint8_t * data, uint32_t len, struct uip_ip_hdr * packet_data);

#endif

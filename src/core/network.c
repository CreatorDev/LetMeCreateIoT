#include "letmecreate/core/network.h"

#include <contiki.h>
#include <contiki-net.h>

#include <stdio.h>
#include <string.h>

#define UIP_IP_BUF ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])

int ipv6_add_address(const char * address, uip_ipaddr_t * addr, unsigned long lifetime)
{
    uip_ipaddr_t tmp_addr;

    if(!addr)
        addr = &tmp_addr;

    if(!uiplib_ipaddrconv(address, addr))
    {
        fprintf(stderr, "Network: Failed to convert IP: %s\n", address);
        return -1;
    }

    if(!uip_ds6_addr_add(addr, lifetime, ADDR_MANUAL))
    {
        fprintf(stderr, "Network: Failed to set IP: %s\n", address);
        return -1;
    }

    return 0;
}

int ipv6_add_default_route(const char * address, unsigned long lifetime)
{
    uip_ipaddr_t addr;

    if(!uiplib_ipaddrconv(address, &addr))
    {
        fprintf(stderr, "Network: Failed to convert IP: %s\n", address);
        return -1;
    }

    if(!uip_ds6_defrt_add(&addr, lifetime))
    {
        fprintf(stderr, "Network: Failed to add a new default route %s\n", address);
        return -1;
    }

    return 0;
}

int ipv6_remove_default_route(const char * address)
{
    uip_ipaddr_t addr;

    if(!uiplib_ipaddrconv(address, &addr))
    {
        fprintf(stderr, "Network: Failed to convert IP: %s\n", address);
        return -1;
    }

    uip_ds6_defrt_t * route = uip_ds6_defrt_lookup(&addr);
    if(route == NULL)
    {
        fprintf(stderr, "Network: Route %s not found\n", address);
        return -1;
    }

    uip_ds6_defrt_rm(route);

    return 0;
}

int ipv6_add_nameserver(const char * address, uint32_t lifetime)
{
    uip_ipaddr_t addr;
    if(!uiplib_ipaddrconv(address, &addr))
    {
        fprintf(stderr, "IPv6: Failed toconvert IP: %s\n", address);
        return -1;
    }

    if(lifetime == NETWORK_INFINITE_LIFETIME)
        lifetime = UIP_NAMESERVER_INFINITE_LIFETIME;

    uip_nameserver_update(&addr, lifetime);
    return 0;
}

int ipv6_remove_nameserver(const char * address)
{
    uip_ipaddr_t addr;
    if(!uiplib_ipaddrconv(address, &addr))
    {
        fprintf(stderr, "IPv6: Failed toconvert IP: %s\n", address);
        return -1;
    }

    uip_nameserver_update(&addr, 0);
    return 0;
}

struct uip_udp_conn * udp_new_connection(uint16_t local_port, uint16_t remote_port, const char * address)
{
    static char * dummy_data = "dummy";
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

    /* Current workaround for the issue with no data being sent until the
       first packet is sent. */
    if(udp_packet_send(conn, dummy_data, strlen(dummy_data)) < 0)
        return NULL;

    return conn;
}

int udp_packet_send(struct uip_udp_conn * connection, const uint8_t * data, uint32_t len)
{
    if(connection == NULL)
    {
        fprintf(stderr, "UDP: Connection cannot be null\n");
        return -1;
    }

    if(data == NULL)
    {
        fprintf(stderr, "UDP: Cannot send null data\n");
        return -1;
    }

    uip_udp_packet_send(connection, data, len);
    tcpip_poll_udp(connection);

    return 0;
}

int udp_packet_sendto(struct uip_udp_conn * connection, const uint8_t * data, uint32_t len,
    const char * address, uint16_t port)
{
    uip_ipaddr_t ipaddr;
    if(connection == NULL)
    {
        fprintf(stderr, "UDP: Connection cannot be null\n");
        return -1;
    }

    if(data == NULL)
    {
        fprintf(stderr, "UDP: Cannot send null data\n");
        return -1;
    }

    if(!uiplib_ipaddrconv(address, &ipaddr))
    {
        printf("Network: Failed to convert IP: %s\n", address);
        return -1;
    }


    uip_udp_packet_sendto(connection, data, len, &ipaddr, UIP_HTONS(port));
    tcpip_poll_udp(connection);

    return 0;
}

int udp_packet_receive(uint8_t * data, uint32_t len, struct uip_ip_hdr * packet_data)
{
    if(data == NULL)
    {
        fprintf(stderr, "UDP: Receive data cannot be null\n");
        return -1;
    }

    if(uip_newdata())
    {
        if(len > uip_datalen())
        {
            len = uip_datalen();
        }

        memcpy(data, (uint8_t*)uip_appdata, len);

        if(packet_data)
        {
           memcpy(packet_data, UIP_IP_BUF, sizeof(struct uip_ip_hdr));
        }

        return len;
    }

    return 0;
}

struct uip_conn * tcp_new_connection(uint16_t remote_port, const char * address)
{
    struct uip_conn * conn = NULL;
    uip_ipaddr_t addr;

    if(!uiplib_ipaddrconv(address, &addr))
    {
        printf("TCP: Failed to convert IP: %s\n", address);
        return NULL;
    }

    conn = tcp_connect(&addr, UIP_HTONS(remote_port), NULL);

    if (!conn)
    {
        printf("TCP: Failed to create connection\n");
        return NULL;
    }

    return conn;
}

int tcp_packet_send(struct uip_conn * connection, const uint8_t * data, uint32_t len)
{
    if (connection == NULL)
    {
        fprintf(stderr, "TCP: Connection cannot be null\n");
        return -1;
    }

    if (data == NULL)
    {
        fprintf(stderr, "TCP: Cannot send null data\n");
        return -1;
    }

    uip_send(data, len);
    tcpip_poll_tcp(connection);

    return 0;
}

int tcp_packet_receive(uint8_t * data, uint32_t len, struct uip_ip_hdr * packet_data)
{
    return udp_packet_receive(data, len, packet_data);
}

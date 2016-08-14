/**
 * @file debug.h
 * @author Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef H_CORE_DEBUG
#define H_CORE_DEBUG

#include <stdarg.h>

#include "letmecreate/core/network.h"

struct uip_udp_conn * __debug_connection;

#ifdef DEBUG_IP
    /** @brief Debug port
      *
      * Default port over which to communicate with the debug server */
    #define DEBUG_PORT 9999

    #undef PRINTF
    /** @brief Initialises debug connection
      *
      * Run at the start of your program to initialise the connection over #DEBUG_PORT
      * Needs the DEBUG_IP CFLAG to be defined */
    #define INIT_NETWORK_DEBUG()  do { if(__debug_connection == NULL)      \
                                   {                                       \
                                       __init_debug_connection();          \
                                       PROCESS_WAIT_UDP_CONNECTED();       \
                                   }    } while(0)
    /** @brief Sends a printf over the network
      *
      * Use the same as printf - will send your message over network as well as
      * print it into the console */
    #define PRINTF(...) do {                                                   \
                            if(__network_printf(__VA_ARGS__) < 0)              \
                                printf("Debug: Failed to send above printf\n");\
                            else                                               \
                                PROCESS_WAIT_UDP_SENT();                       \
                        } while(0)
#else
    #define PRINTF(...) printf(__VA_ARGS__)
    #define INIT_NETWORK_DEBUG()
#endif

void __init_debug_connection();
int __network_printf(const char * format, ...);

#endif

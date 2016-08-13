#ifndef H_CORE_DEBUG
#define H_CORE_DEBUG

#include <stdarg.h>

#include "letmecreate/core/network.h"

struct uip_udp_conn * debug_connection;

#ifdef DEBUG_IP
    #undef PRINTF
    #define INIT_NETWORK_DEBUG()  do { if(debug_connection == NULL)                    \
                                   {                                                   \
                                       init_debug_connection();                        \
                                       PROCESS_WAIT_UDP_CONNECTED();                   \
                                   }    } while(0)

    #define PRINTF(...) do {                                                    \
                            if(network_printf(__VA_ARGS__) < 0)                 \
                                printf("Debug: Failed to send above printf\n"); \
                            else                                                \
                                PROCESS_WAIT_UDP_SENT();                        \
                        } while(0)
#else
    #define PRINTF(...) printf(__VA_ARGS__)
    #define INIT_NETWORK_DEBUG()
#endif

void init_debug_connection();
int network_printf(const char * format, ...);

#endif

#include "uart.h"

#include <stdio.h>
#include <contiki.h>
#include <pic32_uart.h>
#include <pic32_clock.h>
#include <lib/ringbuf.h>

#include "letmecreate/core/common.h"

#define DEFAULT_BAUDRATE UART_BD_9600

#define UART_BUFFER_SIZE 128

/* This is assumed based on the fact that CLOCK_CONF_SECOND is 1024 */
#define CLOCK_CONF_MS 1

#if(UART_BUFFER_SIZE > 128)
    #error "UART buffer size cannot be larger than 128 bytes"
#endif

#ifndef __USE_UART_PORT3__
    #warning "__USE_UART_PORT3__ not defined, UART wrapper support for Mikrobus will not be enabled"
#else

static uint32_t current_baud_rate = DEFAULT_BAUDRATE;
static uint32_t current_timeout = UART_TIMEOUT_NEVER;
static uint8_t uart_buffer[UART_BUFFER_SIZE];
static struct ringbuf uart_ring_buffer;
static int (*previous_uart_handler)(unsigned char);

static int uart_handler(unsigned char c)
{
    if(!ringbuf_put(&uart_ring_buffer, (uint8_t)c))
    {
        fprintf(stderr, "uart: Ring buffer is full, missed data\n");
        return 0;
    }

    return 1;
}

#ifndef __USE_UART_PORT3_FOR_DEBUG__
static void dummy_callback(unsigned char c)
{
}


UART_INTERRUPT(3, 1, dummy_callback);
#endif // __USE_UART_PORT3_FOR_DEBUG__

#endif

int uart_init(void)
{
#ifndef __USE_UART_PORT3__
    fprintf(stderr, "uart: __USE_UART_PORT3__ not defined\n");
    return -1;
#else
    if(pic32_uart3_init(DEFAULT_BAUDRATE, 0))
    {
        fprintf(stderr, "uart: Init failed\n");
        return -1;
    }

    current_baud_rate = DEFAULT_BAUDRATE;

    ringbuf_init(&uart_ring_buffer, uart_buffer, UART_BUFFER_SIZE);
    previous_uart_handler = uart3_input_handler;
    uart3_set_input(uart_handler);

    return 0;
#endif
}

void uart_select_bus(uint8_t mikrobus_index)
{
#ifndef __USE_UART_PORT3__
    fprintf(stderr, "uart: __USE_UART_PORT3__ not defined\n");
#else
    if(check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "uart: Invalid mikrobus\n");
    }
#endif
}

uint8_t uart_get_current_bus(void)
{
#ifndef __USE_UART_PORT3__
    fprintf(stderr, "uart: __USE_UART_PORT3__ not defined\n");
#endif

    return MIKROBUS_1;
}

int uart_set_baudrate(uint32_t baudrate)
{
#ifndef __USE_UART_PORT3__
    fprintf(stderr, "uart: __USE_UART_PORT3__ not defined\n");
    return -1;
#else
    U3BRG = pic32_clock_calculate_brg(4, baudrate);
    current_baud_rate = baudrate;

    return 0;
#endif
}

int uart_get_baudrate(uint32_t *baudrate)
{
#ifndef __USE_UART_PORT3__
    fprintf(stderr, "uart: __USE_UART_PORT3__ not defined\n");
    return -1;
#else
    if(baudrate == NULL)
    {
        fprintf(stderr, "uart: Cannot set baudrate using null pointer.\n");
        return -1;
    }

    *baudrate = current_baud_rate;

    return 0;
#endif
}

int uart_get_real_baudrate(uint32_t *baudrate)
{
#ifndef __USE_UART_PORT3__
    fprintf(stderr, "uart: __USE_UART_PORT3__ not defined\n");
    return -1;
#else
    if(baudrate == NULL)
    {
        fprintf(stderr, "uart: Cannot set baudrate using null pointer.\n");
        return -1;
    }

    /* From section 21. UART:
     *
     * If BRGH = 0:
     *                    Fpb
     * Baud rate = -----------------
     *              16 * (U3BRG + 1)
     *
     * If BRGH = 1:
     *                    Fpb
     * Baud rate = -----------------
     *              4 * (U3BRG + 1)
     */
    uint32_t x = (U3MODE & _U3MODE_BRGH_MASK) ? 4 : 16;
    *baudrate = pic32_clock_get_peripheral_clock() / (x * (U3BRG + 1));

    return 0;
#endif
}

int uart_send(const uint8_t *buffer, uint32_t count)
{
#ifndef __USE_UART_PORT3__
    fprintf(stderr, "uart: __USE_UART_PORT3__ not defined\n");
    return -1;
#else
    uint32_t i;
    for(i = 0; i < count; i++)
    {
        if(pic32_uart3_write(buffer[i]))
        {
            fprintf(stderr, "uart: Send error\n");
            return -1;
        }
    }

    return 0;
#endif
}

int uart_receive(uint8_t *buffer, uint32_t count)
{
#ifndef __USE_UART_PORT3__
    fprintf(stderr, "uart: __USE_UART_PORT3__ not defined\n");
    return -1;
#else
    uint32_t i = 0;
    int data = -1;
    rtimer_clock_t start;
    start = RTIMER_NOW();
    for(i = 0; i < count; i++)
    {
        while((data = ringbuf_get(&uart_ring_buffer)) < 0)
        {
            if(current_timeout == UART_TIMEOUT_NEVER)
                continue;

            if(RTIMER_CLOCK_LT(start + current_timeout, RTIMER_NOW()))
            {
                fprintf(stderr, "uart: Read timed out\n");
                return -1;
            }
        }

        buffer[i] = (uint8_t)data;
    }
    return 0;
#endif
}

uint32_t uart_get_timeout(void)
{
#ifndef __USE_UART_PORT3__
    fprintf(stderr, "uart: __USE_UART_PORT3__ not defined\n");
    return 0;
#else
    return current_timeout;
#endif
}

void uart_set_timeout(uint32_t timeout)
{
#ifndef __USE_UART_PORT3__
    fprintf(stderr, "uart: __USE_UART_PORT3__ not defined\n");
#else
    current_timeout = timeout;
#endif
}

int uart_release(void)
{
#ifndef __USE_UART_PORT3__
    fprintf(stderr, "uart: __USE_UART_PORT3__ not defined\n");
    return -1;
#elif defined(__USE_UART_PORT3_FOR_DEBUG__)
    if(previous_uart_handler == uart_handler)
        UART_SET_INPUT(NULL);
    else
        UART_SET_INPUT(previous_uart_handler);

    /* If used for debug, do not turn off module */
    return 0;
#endif
    /* Disable interrupts */
    IEC1CLR = _IEC1_U3EIE_MASK | _IEC2_U3TXIE_MASK | _IEC1_U3RXIE_MASK;
    IFS1CLR = _IFS1_U3EIF_MASK | _IFS2_U3TXIF_MASK | _IFS1_U3RXIF_MASK;

    /* Clear priority bits */
    IPC9CLR = _IPC9_U3IP_MASK | _IPC9_U3IS_MASK;

    U3MODE = 0;
    U3STA = 0;

    return 0;
}

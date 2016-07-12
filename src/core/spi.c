#ifndef H_CORE_SPI
#define H_CORE_SPI

#include "spi.h"

#include <stdio.h>
#include <unistd.h>

#include <pic32_clock.h>
#include <pic32_spi.h>

#define DEFAULT_BAUD_RATE 100000

uint8_t spi_init()
{
    if(pic32_spi1_init(DEFAULT_BAUD_RATE, SPI_DEFAULT))
        return 1;

    return 0;
}

uint8_t spi_release()
{
    if(pic32_spi1_close())
        return 1;

    return 0;
}

uint8_t spi_set_speed(uint32_t speed)
{
    SPI1BRG = pic32_clock_calculate_brg(2, speed);

    return 0;
}

uint8_t spi_transfer(const uint8_t * tx_buffer, uint8_t * rx_buffer, uint8_t len)
{
    if(len == 0)
    {
        printf("SPI: Length of transfer is zero\n");
        return 1;
    }

    if(!tx_buffer && !rx_buffer)
    {
        printf("SPI: Buffers are empty\n");
        return 1;
    }

    if(tx_buffer)
    {
        if(pic32_spi1_write(tx_buffer[0], 1))
        {
            printf("SPI: Failed to write address\n");
            return 1;
        }
    }

    if(len > 1)
    {
        if(rx_buffer)
        {
            if(pic32_spi1_read(rx_buffer, len - 1))
            {
                printf("SPI: Failed to read the buffer\n");
                return 1;
            }
        }
        else
        {
            if(pic32_spi1_write(tx_buffer + 1, len - 1))
            {
                printf("SPI: Failed to write the buffer\n");
                return 1;
            }
        }
    }
    return 0;
}

#endif

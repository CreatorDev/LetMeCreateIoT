#ifndef H_CORE_SPI
#define H_CORE_SPI

#include "spi.h"

#include <stdio.h>

#include <pic32_clock.h>
#include <pic32_spi.h>

#define DEFAULT_BAUD_RATE (100000)

#define SPI_READ_BIT            (0x80)
#define SPI_MULTIPLE_BYTE_BIT   (0x40)

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


uint8_t spi_write(uint8_t reg_address, const uint8_t * buffer, uint8_t len)
{
    if(!buffer || len == 0)
    {
        printf("SPI: No data to write\n");
        return 1;
    }

    if(len > 1)
        reg_address |= SPI_MULTIPLE_BYTE_BIT;

    if(pic32_spi1_write(&reg_address, 1))
    {
        printf("SPI: Failed to write registry address\n");
        return 1;
    }

    if(pic32_spi1_write(buffer, len))
    {
        printf("SPI: Failed to write data\n");
        return 1;
    }

    return 0;
}


uint8_t spi_read(uint8_t reg_address, uint8_t * buffer, uint8_t len)
{
    if(!buffer || len == 0)
    {
        printf("SPI: No data to read\n");
        return 1;
    }

    reg_address |= SPI_READ_BIT;
    if(len > 1)
        reg_address |= SPI_MULTIPLE_BYTE_BIT;

    if(pic32_spi1_write(&reg_address, 1))
    {
        printf("SPI: Failed to write registry address\n");
        return 1;
    }

    if(pic32_spi1_read(buffer, len))
    {
        printf("SPI: Failed to read data\n");
        return 1;
    }


    return 0;
}

#endif

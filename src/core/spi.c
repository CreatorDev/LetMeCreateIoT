#ifndef H_CORE_SPI
#define H_CORE_SPI

#include "spi.h"

#include <stdio.h>

#include <pic32_clock.h>
#include <pic32_spi.h>
#include <pic32_gpio.h>

#define DEFAULT_BAUD_RATE       (100000)

#define SPI_READ_BIT            (0x80)
#define SPI_MULTIPLE_BYTE_BIT   (0x40)

uint8_t spi_init(uint8_t mode)
{
    uint32_t flags = SPI_MASTER;
    switch(mode)
    {
        case 0:
        flags |= SPI_SDO_ON_CLOCK_TO_IDLE;
        break;
        case 1:
        break;
        case 2:
        flags |= SPI_CLOCK_IDLE_HIGH | SPI_SDO_ON_CLOCK_TO_IDLE;
        break;
        case 3:
        flags |= SPI_CLOCK_IDLE_HIGH;
        break;
        default:
        printf("SPI: Invalid mode\n");
        return 1;
    }

    if(pic32_spi1_init(DEFAULT_BAUD_RATE, flags))
        return 1;

    GPIO_CONFIGURE_AS_DIGITAL(E, 5);
    GPIO_CONFIGURE_AS_OUTPUT(E, 5);
    GPIO_SET(E, 5);

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

uint8_t spi_start_transfer()
{
    GPIO_CLR(E, 5);

    return 0;
}

uint8_t spi_end_transfer()
{
    GPIO_SET(E, 5);

    return 0;
}

uint8_t spi_write(const uint8_t * buffer, uint8_t len)
{
    if(!buffer || len == 0)
    {
        printf("SPI: No data to write\n");
        return 1;
    }

    if(pic32_spi1_write(buffer, len))
    {
        printf("SPI: Failed to write\n");
        return 1;
    }

    return 0;
}


uint8_t spi_read(uint8_t * buffer, uint8_t len)
{
    if(!buffer || len == 0)
    {
        printf("SPI: No data to read\n");
        return 1;
    }

    if(pic32_spi1_read(buffer, len))
    {
        printf("SPI: Failed to read data\n");
        return 1;
    }

    return 0;
}

uint8_t spi_write_registers(uint8_t reg_address, const uint8_t * buffer, uint8_t len)
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


uint8_t spi_read_registers(uint8_t reg_address, uint8_t * buffer, uint8_t len)
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

uint8_t spi_write_register(uint8_t reg_address, uint8_t byte)
{
    return spi_write_registers(reg_address, &byte, 1);
}

uint8_t spi_read_register(uint8_t reg_address, uint8_t byte)
{
    return spi_read_registers(reg_address, &byte, 1);
}


#endif

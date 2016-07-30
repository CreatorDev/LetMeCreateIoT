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
        return -1;
    }

    if(pic32_spi1_init(DEFAULT_BAUD_RATE, flags))
        return -1;

    GPIO_CONFIGURE_AS_DIGITAL(E, 5);
    GPIO_CONFIGURE_AS_OUTPUT(E, 5);
    GPIO_SET(E, 5);

    return 0;
}

uint8_t spi_release()
{
    if(pic32_spi1_close())
        return -1;

    return 0;
}

uint8_t spi_set_speed(uint32_t speed)
{
    SPI1BRG = pic32_clock_calculate_brg(2, speed);

    return 0;
}

static uint8_t spi_start_transfer()
{
    GPIO_CLR(E, 5);

    return 0;
}

static uint8_t spi_end_transfer()
{
    GPIO_SET(E, 5);

    return 0;
}

static uint8_t spi_write(const uint8_t * buffer, uint32_t len)
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

static uint8_t spi_read_registers(uint8_t reg_address, uint8_t * buffer, uint32_t len)
{
    if(!buffer || len == 0)
    {
        printf("SPI: No data to read\n");
        return -1;
    }

    reg_address |= SPI_READ_BIT;
    if(len > 1)
        reg_address |= SPI_MULTIPLE_BYTE_BIT;

    if(pic32_spi1_write(&reg_address, 1))
    {
        printf("SPI: Failed to write registry address\n");
        return -1;
    }

    if(pic32_spi1_read(buffer, len))
    {
        printf("SPI: Failed to read data\n");
        return -1;
    }


    return 0;
}


uint8_t spi_transfer(const uint8_t * tx_buffer, uint8_t * rx_buffer, uint32_t len)
{
    if((!tx_buffer && !rx_buffer) || len == 0)
    {
        printf("SPI: No data to write\n");
        return -1;
    }

    spi_start_transfer();
    if(tx_buffer && !rx_buffer)
    {
        if(spi_write(tx_buffer, len) < 0)
        {
           spi_end_transfer();
           return -1;
        }
    }
    else if(tx_buffer && rx_buffer)
    {
        if(spi_read_registers(tx_buffer[0], &rx_buffer[1], len - 1) < 0)
        {
            spi_end_transfer();
            return -1;
        }
    }
    else
    {
        printf("SPI: Unsupported transfer case\n");
    }
    return 0;
}




#endif

#ifndef H_CORE_SPI
#define H_CORE_SPI

#include "spi.h"

#include <stdio.h>

#include <pic32_clock.h>
#include <pic32_spi.h>
#include <pic32_gpio.h>

#include "letmecreate/core/gpio.h"

#define DEFAULT_BAUD_RATE       (1000000)

#define SPI_READ_BIT            (0x80)
#define SPI_MULTIPLE_BYTE_BIT   (0x40)

int spi_init(uint8_t mode)
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
        fprintf(stderr, "SPI: Invalid mode\n");
        return -1;
    }

    if(pic32_spi1_init(DEFAULT_BAUD_RATE, flags))
        return -1;

    gpio_init(GPIO_CS);
    gpio_set_direction(GPIO_CS, GPIO_OUTPUT);
    gpio_set_value(GPIO_CS, 1);

    return 0;
}

int spi_set_speed(uint32_t speed)
{
    SPI1BRG = pic32_clock_calculate_brg(2, speed);

    return 0;
}

static int spi_start_transfer()
{
    return gpio_set_value(GPIO_CS, 0);
}

static int spi_end_transfer()
{
    return gpio_set_value(GPIO_CS, 1);
}

int spi_transfer(const uint8_t * tx_buffer, uint8_t * rx_buffer, uint32_t len)
{
    if((!tx_buffer && !rx_buffer) || len == 0)
    {
        fprintf(stderr, "SPI: No data to transfer\n");
        return -1;
    }

    spi_start_transfer();
    if(pic32_spi1_transfer(tx_buffer, rx_buffer, len))
    {
        spi_end_transfer();
        return -1;
    }
    spi_end_transfer();
    return len;
}

int spi_release()
{
    if(pic32_spi1_close())
        return -1;

    return 0;
}

#endif

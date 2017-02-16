#include "letmecreate/core/spi.h"

#include <stdio.h>
#include <limits.h>

#include <pic32_clock.h>
#include <pic32_spi.h>
#include <pic32_gpio.h>

#include "letmecreate/core/common.h"
#include "letmecreate/core/gpio.h"

#define DEFAULT_BAUD_RATE       (SPI_2M73)

#define SPI_READ_BIT            (0x80)
#define SPI_MULTIPLE_BYTE_BIT   (0x40)

int spi_init(void)
{
    uint32_t flags = SPI_MASTER;

    if(pic32_spi1_init(DEFAULT_BAUD_RATE, flags))
    {
        fprintf(stderr, "SPI: Failed to init SPI\n");
        return -1;
    }

    if(spi_set_mode(MIKROBUS_1, SPI_MODE_3) < 0)
    {
        fprintf(stderr, "SPI: Failed to set mode\n");
        return -1;
    }

    gpio_init(GPIO_CS);
    gpio_set_direction(GPIO_CS, GPIO_OUTPUT);
    gpio_set_value(GPIO_CS, 1);

    return 0;
}

int spi_set_mode(uint8_t mikrobus_index, uint32_t mode)
{
    if(check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "SPI: Invalid mikrobus\n");
        return -1;
    }

    int flags = SPI_MASTER;
    switch(mode)
    {
        case SPI_MODE_0:
        flags |= SPI_SDO_ON_CLOCK_TO_IDLE;
        break;
        case SPI_MODE_1:
        break;
        case SPI_MODE_2:
        flags |= SPI_CLOCK_IDLE_HIGH | SPI_SDO_ON_CLOCK_TO_IDLE;
        break;
        case SPI_MODE_3:
        flags |= SPI_CLOCK_IDLE_HIGH;
        break;
        default:
        fprintf(stderr, "SPI: Invalid mode\n");
        return -1;
    }

    SPI1CONCLR = SPI_MASTER | SPI_SDO_ON_CLOCK_TO_IDLE | SPI_CLOCK_IDLE_HIGH;
    SPI1CONSET = flags | _SPI1CON_ON_MASK;

    return 0;
}

int spi_set_speed(uint32_t speed)
{
    SPI1BRG = pic32_clock_calculate_brg(2, speed);

    return 0;
}

int spi_select_bus(uint8_t mikrobus_index)
{
    if(check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "SPI: Invalid mikrobus index\n");
        return -1;
    }

    return 0;
}

uint8_t spi_get_current_bus(void)
{
    return MIKROBUS_1;
}

static int spi_start_transfer(void)
{
    return gpio_set_value(GPIO_CS, 0);
}

static int spi_end_transfer(void)
{
    return gpio_set_value(GPIO_CS, 1);
}

int spi_get_maximum_tranfer_length(uint32_t *transfer_length_limit)
{
    if(transfer_length_limit == NULL)
    {
        fprintf(stderr, "SPI: Cannot store transfer length limit using null pointer\n");
        return -1;
    }

    *transfer_length_limit = UINT_MAX;
    return 0;
}

int spi_transfer(const uint8_t * tx_buffer, uint8_t * rx_buffer, uint32_t len)
{
    if(!tx_buffer && !rx_buffer)
    {
        fprintf(stderr, "SPI: No data to transfer\n");
        return -1;
    }

    if (len == 0)
        return 0;

    spi_start_transfer();
    if(pic32_spi1_transfer(tx_buffer, rx_buffer, len))
    {
        fprintf(stderr, "SPI: Transfer failed\n");
        spi_end_transfer();
        return -1;
    }
    spi_end_transfer();
    return len;
}

int spi_release()
{
    if(pic32_spi1_close())
    {
        fprintf(stderr, "SPI: Failed to release SPI\n");
        return -1;
    }

    return 0;
}

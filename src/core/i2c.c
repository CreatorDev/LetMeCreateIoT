#include "i2c.h"

#include <stdio.h>
#include <contiki.h>
#include <pic32_i2c.h>
#include <p32xxxx.h>

#define I2C_DEFAULT_FREQUENCY (100000)

#define I2C_16BIT_ADDRESSING  (0xF000)

#define I2C_READ_MODE         (0x01)
#define I2C_WRITE_MODE        (0x00)

static int i2c_send_address(uint16_t address, uint8_t read_byte)
{
    read_byte = read_byte & 0x01;

    if(!(0xff00 & address))
    {
        address = (address << 1) | read_byte;

        if(i2c1_send_byte(address))
            return -1;

        return 0;
    }

    uint8_t upper_byte = (I2C_16BIT_ADDRESSING) | (address & 0xff00) | read_byte;
    uint8_t lower_byte = address & 0x00ff;

    if(i2c1_send_byte(upper_byte) || i2c1_send_byte(lower_byte))
        return -1;

    return 0;
}

int i2c_init()
{
    if(i2c1_init())
        return -1;

    if(i2c1_set_frequency(I2C_DEFAULT_FREQUENCY))
        return -1;

    if(i2c1_master_enable())
        return -1;


    return 0;
}

int i2c_release()
{
    if(i2c1_master_disable())
        return -1;

    return 0;
}

int i2c_set_frequency(uint32_t frequency)
{
    if(i2c1_set_frequency(frequency))
        return -1;

    return 0;
}

int i2c_write(uint16_t address, const uint8_t * bytes, uint32_t length)
{
    if(!bytes)
    {
        printf("I2C: Cannot write null bytes\n");
        return -1;
    }

    if(!length)
    {
        printf("I2C: Length cannot be 0\n");
        return -1;
    }

    if(i2c1_send_start())
        return -1;

    if(i2c_send_address(address, I2C_WRITE_MODE) < 0)
        return -1;

    if(i2c1_send_bytes(bytes, length))
        return -1;

    if(i2c1_send_stop())
        return -1;

    return 0;
}

int i2c_read(uint16_t address, uint8_t * bytes, uint32_t length)
{
    if(!bytes)
    {
        printf("I2C: Cannot read to null bytes\n");
        return -1;
    }

    if(!length)
    {
        printf("I2C: Length cannot be 0\n");
        return -1;
    }



    if(i2c1_send_start())
        return -1;

    if(i2c_send_address(address, I2C_READ_MODE) < 0)
        return -1;

    if(i2c1_set_nack(1))
        return -1;

    if(i2c1_receive_bytes(bytes, length))
        return -1;

    if(i2c1_send_stop())
        return -1;

    return 0;
}

int i2c_write_byte(uint16_t slave_address, uint8_t data)
{
    return i2c_write(slave_address, &data, 1);
}

int i2c_read_byte(uint16_t slave_address, uint8_t *data)
{
    return i2c_read(slave_address, data, 1);
}

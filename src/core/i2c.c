#include "i2c.h"

#include <stdio.h>
#include <contiki.h>
#include <pic32_i2c.h>
#include <p32xxxx.h>

#define I2C_DEFAULT_FREQUENCY (100000)

#define I2C_READ_MODE         (0x01)
#define I2C_WRITE_MODE        (0x00)

static int i2c_send_address(uint16_t address, uint8_t read_byte)
{
    read_byte = read_byte & 0x01;

    if(!(0xff00 & address))
    {
        uint8_t small_address = ((0xff & address) << 1) | read_byte;

        if(i2c1_send_byte(small_address))
            return -1;

        return 0;
    }

    uint8_t upper_byte = (0xF0) | ((address & 0xff00) << 1) | read_byte;
    uint8_t lower_byte = address & 0x00ff;

    if(i2c1_send_byte(upper_byte) || i2c1_send_byte(lower_byte))
        return -1;

    return 0;
}

int i2c_init()
{
    if(i2c1_init())
    {
        fprintf(stderr, "I2C: Failed to initialise\n");
        return -1;
    }

    if(i2c1_set_frequency(I2C_DEFAULT_FREQUENCY))
    {
        fprintf(stderr, "I2C: Failed to set frequency\n");
        return -1;
    }

    if(i2c1_master_enable())
    {
        fprintf(stderr, "I2C: Failed to enable master\n");
        return -1;
    }


    return 0;
}

int i2c_release()
{
    if(i2c1_master_disable())
    {
        fprintf(stderr, "I2C: Failed to disable master\n");
        return -1;
    }

    return 0;
}

int i2c_set_frequency(uint32_t frequency)
{
    if(i2c1_set_frequency(frequency))
    {
        fprintf(stderr, "I2C: Failed to set frequency\n");
        return -1;
    }

    return 0;
}

int i2c_write(uint16_t address, const uint8_t * bytes, uint32_t length)
{
    if(!bytes)
    {
        fprintf(stderr, "I2C: Cannot write null bytes\n");
        return -1;
    }

    if(length == 0)
        return 0;

    if(i2c1_send_start())
    {
        fprintf(stderr, "I2C: Send start failed\n");
        return -1;
    }

    if(i2c_send_address(address, I2C_WRITE_MODE) < 0)
    {
        fprintf(stderr, "I2C: Send address failed\n");
        return -1;
    }

    if(i2c1_send_bytes(bytes, length))
    {
        fprintf(stderr, "I2C: Send bytes failed\n");
        return -1;
    }

    if(i2c1_send_stop())
    {
        fprintf(stderr, "I2C: Send stop failed\n");
        return -1;
    }

    return length;
}

int i2c_read(uint16_t address, uint8_t * bytes, uint32_t length)
{
    if(!bytes)
    {
        fprintf(stderr, "I2C: Cannot read to null bytes\n");
        return -1;
    }

    if(length == 0)
        return 0;

    if(i2c1_send_start())
    {
        fprintf(stderr, "I2C: Send start failed\n");
        return -1;
    }

    if(i2c_send_address(address, I2C_READ_MODE) < 0)
    {
        fprintf(stderr, "I2C: Send address failed\n");
        return -1;
    }

    if(i2c1_set_nack(1))
    {
        fprintf(stderr, "I2C: Set nack failed\n");
        return -1;
    }

    if(i2c1_receive_bytes(bytes, length))
    {
        fprintf(stderr, "I2C: Receive bytes failed\n");
        return -1;
    }

    if(i2c1_send_stop())
    {
        fprintf(stderr, "I2C: Send stop failed\n");
        return -1;
    }

    return length;
}

int i2c_write_byte(uint16_t slave_address, uint8_t data)
{
    return i2c_write(slave_address, &data, 1);
}

int i2c_read_byte(uint16_t slave_address, uint8_t *data)
{
    return i2c_read(slave_address, data, 1);
}

int i2c_write_register(uint16_t address, uint8_t reg_address, uint8_t value)
{
    uint8_t buffer[2] = { reg_address, value };
    return i2c_write(address, buffer, sizeof(buffer));
}

int i2c_read_register(uint16_t address, uint8_t reg_address, uint8_t *data)
{
    if (i2c_write_byte(address, reg_address) < 0)
    {
        fprintf(stderr, "I2C: Failed to read register\n");
        return -1;
    }

    return i2c_read_byte(address, data);
}

int i2c_read_16b_register(uint16_t address,
                          uint8_t reg_low_address, uint8_t reg_high_address,
                          uint16_t *data)
{
    uint8_t low = 0, high = 0;

    if (data == NULL)
    {
        fprintf(stderr, "I2C: Data cannot be null\n");
        return -1;
    }

    if (i2c_read_register(address, reg_low_address, &low) < 0)
        return -1;

    if (i2c_read_register(address, reg_high_address, &high) < 0)
        return -1;

    *data = high;
    *data <<= 8;
    *data |= low;

    return 0;
}


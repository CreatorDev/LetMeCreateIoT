#include <contiki.h>
#include <pic32_i2c.h>
#include <p32xxxx.h>
#incldue "core/i2c.h"

#define I2C_DEFAULT_FREQUENCY 100000

uint8_t i2c_init()
{
    if(i2c1_init())
        return 1;

    if(i2c1_set_frequency(I2C_DEFAULT_FREQUENCY))
        return 1;

    if(i2c1_master_enable())
        return 1;


    return 0;
}

uint8_t i2c_release()
{
    return i2c1_master_disable();
}

uint8_t i2c_set_frequency(uint32_t frequency)
{
    return i2c1_set_frequency(frequency);
}

uint8_t i2c_write(uint8_t address, uint8_t reg_address, uint8_t * bytes, uint8_t length)
{
    if(!bytes)
    {
        printf("I2C: Cannot write null bytes\n");
        return 1;
    }

    if(!length)
    {
        printf("I2C: Length cannot be 0\n");
        return 1;
    }

    address = (address << 1);

    if(i2c1_send_start())
        return 1;

    if(i2c1_send_byte(address))
        return 1;

    if(i2c1_send_byte(reg_address))
        return 1;

    if(i2c1_send_bytes(bytes, length))
        return 1;

    if(i2c1_send_stop())
        return 1;

    return 0;
}

uint8_t i2c_read(uint8_t address, uint8_t reg_address, uint8_t * bytes, uint8_t length)
{
    if(!bytes)
    {
        printf("I2C: Cannot read to null bytes\n");
        return 1;
    }

    if(!length)
    {
        printf("I2C: Length cannot be 0\n");
        return 1;
    }


    address = address << 1;

    if(i2c1_send_start())
        return 1;

    if(i2c1_send_byte(address))
        return 1;

    if(i2c1_send_byte(reg_address))
        return 1;

    if(i2c1_send_repeated_start())
        return 1;

    address |= 0x01;

    if(i2c1_send_byte(address))
        return 1;

    if(i2c1_set_nack(1))
        return 1;

    if(i2c1_receive_bytes(bytes, length))
        return 1;

    if(i2c1_send_stop())
        return 1;

    return 0;
}

#include <stdio.h>
#include <string.h>

#include <letmecreate/core/spi.h>


/* Control registers */
#define BW_RATE_REG         (0x2C)
#define POWER_CTRL_REG      (0x2D)
#define DATA_FORMAT_REG     (0x31)
#define FIFO_CTRL_REG       (0x38)

/* Control values for */
#define DATA_RATE           (0x0F)  /* 3.2kHz */
#define MEASURE_EN          (0x08)
#define SLEEP_EN            (0x04)
#define WAKEUP_DATA_RATE    (0x03)  /* 8 readings/sec in sleep mode */
#define FULL_RES_EN         (0x08)
#define RANGE               (0x03)  /* max range +- 16g */

/* Output registers */
#define DATAX0_REG          (0x32)
#define DATAX1_REG          (0x33)
#define DATAY0_REG          (0x34)
#define DATAY1_REG          (0x35)
#define DATAZ0_REG          (0x36)
#define DATAZ1_REG          (0x37)

#define G_PER_LSB           (0.004f) /* 4mg/LSB */

#define SPI_MODE            (0x03)

uint8_t accel_click_enable()
{
    spi_init(SPI_MODE);

    spi_start_transfer();

    if (spi_write_register(POWER_CTRL_REG, MEASURE_EN | WAKEUP_DATA_RATE))
    {
        printf("accel: Failed to enable device.\n");
        return 1;
    }

    if (spi_write_register(FIFO_CTRL_REG, 0))
    {
        printf("accel: Failed to set fifo settings.\n");
        return 1;
    }

    if (spi_write_register(BW_RATE_REG, DATA_RATE))
    {
        printf("accel: Failed to set data rate.\n");
        return 1;
    }

    if (spi_write_register(DATA_FORMAT_REG, FULL_RES_EN | RANGE))
    {
        printf("accel: Failed to set data format.\n");
        return 1;
    }

    spi_end_transfer();

    return 0;
}


uint8_t accel_click_get_measure(float * accelX, float * accelY, float * accelZ)
{
    int16_t x, y, z;
    uint8_t buffer[6];

    if(!accelX || !accelY || !accelZ)
    {
        printf("accel: Passed a null pointer for data\n");
        return 1;
    }

    spi_start_transfer();
    if(spi_read_registers(DATAX0_REG, buffer, 6))
    {
        printf("accel: Failed to read\n");
        return 1;
    }

    memcpy(&x, &buffer[0], 2);
    memcpy(&y, &buffer[2], 2);
    memcpy(&z, &buffer[4], 2);

    *accelX = (float)x * G_PER_LSB;
    *accelY = (float)y * G_PER_LSB;
    *accelZ = (float)z * G_PER_LSB;

    spi_end_transfer();
    return 0;
}


uint8_t accel_click_disable()
{
    spi_start_transfer();
    if (spi_write_register(POWER_CTRL_REG, SLEEP_EN) < 0)
    {
        printf("accel: Failed to shutdown device.\n");
        return 1;
    }
    spi_end_transfer();

    return 0;
}

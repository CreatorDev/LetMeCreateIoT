#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <letmecreate/click/thermo3.h>
#include <letmecreate/core/i2c.h>

#define TMP102_ADDRESS                  (0x48 | (last_address_bit & 0x01))
#define TEMPERATURE_REG_ADDRESS         (0x00)
#define CONFIGURATION_REG_ADDRESS       (0x01)
#define TEMPERATURE_HIGH_REG_ADDRESS    (0x03)

#define DEGREES_CELCIUS_PER_LSB         (0.0625f)
#define SHUTDOWN_MODE           (0x01)
#define CONVERSION_RATE         (0xC0)

static bool enabled = false;
static uint8_t last_address_bit = 0;

int thermo3_click_enable(uint8_t add_bit)
{
    uint8_t buffer[3];

    last_address_bit = add_bit;

    buffer[0] = CONFIGURATION_REG_ADDRESS;
    buffer[1] = 0;
    buffer[2] = CONVERSION_RATE;
    if (i2c_write(TMP102_ADDRESS, buffer, sizeof(buffer)) < 0) {
        fprintf(stderr, "thermo3: Failed to configure and enable sensor.\n");
        return -1;
    }

    enabled = true;

    return 0;
}

int thermo3_click_get_temperature(float *temperature)
{
    uint8_t buffer[2];

    if (temperature == NULL) {
        fprintf(stderr, "thermo3: Cannot store temperature using null pointer.\n");
        return -1;
    }

    if (enabled == false) {
        fprintf(stderr, "thermo3: Cannot get temperature from disabled sensor.\n");
        return -1;
    }

    if (i2c_write_byte(TMP102_ADDRESS, TEMPERATURE_REG_ADDRESS) < 0) {
        fprintf(stderr, "thermo3: Failed to request temperature from sensor.\n");
        return -1;
    }

    if (i2c_read(TMP102_ADDRESS, buffer, sizeof(buffer)) < 0) {
        fprintf(stderr, "thermo3: Failed to read temperature from sensor.\n");
        return -1;
    }

    *temperature = (float)(buffer[0]);
    *temperature += ((float)(buffer[1] >> 4)) * DEGREES_CELCIUS_PER_LSB;

    return 0;
}


int thermo3_click_disable(void)
{
    uint8_t buffer[3];

    buffer[0] = CONFIGURATION_REG_ADDRESS;
    buffer[1] = SHUTDOWN_MODE;
    buffer[2] = 0;
    if (i2c_write(TMP102_ADDRESS, buffer, sizeof(buffer)) < 0) {
        fprintf(stderr, "thermo3: Failed to shutdown sensor.\n");
        return -1;
    }

    enabled = false;

    return 0;
}

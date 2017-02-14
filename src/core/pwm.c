#include <pic32_pwm.h>
#include <pic32_gpio.h>
#include <stdbool.h>
#include <stdio.h>
#include <letmecreate/core/pwm.h>
#include <letmecreate/core/common.h>

/* Default period is 333333ns, in other words 3KHz */
#define DEFAULT_PERIOD          (333333)

static bool enabled[MIKROBUS_COUNT];

int pwm_init(uint8_t mikrobus_index)
{
    if (check_valid_mikrobus(mikrobus_index) < 0)
        return -1;

    /* Configure the PWM pin as GPIO output low,
     * So, when the PWM driver is not driving the pin, the pin stays low.
     */
    GPIO_CONFIGURE_AS_DIGITAL(B, 8);
    GPIO_CONFIGURE_AS_OUTPUT(B, 8);
    GPIO_CLR(B, 8);

    if (!pic32_pwm5_init(DEFAULT_PERIOD)
    &&  !pic32_pwm5_set_duty_cycle(DEFAULT_PERIOD/2)
    &&  !pwm_disable(mikrobus_index))
        return 0;

    return -1;
}

int pwm_enable(uint8_t mikrobus_index)
{
    if (check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "pwm: Invalid mikrobus index.\n");
        return -1;
    }

    if (pic32_pwm5_enable() != 0)
        return -1;

    enabled[mikrobus_index] = true;

    return 0;
}

int pwm_set_duty_cycle(uint8_t mikrobus_index, float percentage)
{
    if (check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "pwm: Invalid mikrobus index.\n");
        return -1;
    }

    if (percentage < 0.f || percentage > 100.f) {
        fprintf(stderr, "pwm: Invalid percentage (must be in range 0..100).\n");
        return -1;
    }

    pic32_pwm5_set_duty_cycle(pic32_pwm5_get_maximum_duty_cycle() * (percentage/100.f));

    return 0;
}

int pwm_get_duty_cycle(uint8_t mikrobus_index, float *percentage)
{
    uint32_t max_duty_cycle;

    if (check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "pwm: Invalid mikrobus index\n");
        return -1;
    }
    if (percentage == NULL)
    {
        fprintf(stderr, "pwm: percentage cannot be null\n");
        return -1;
    }

    max_duty_cycle = pic32_pwm5_get_maximum_duty_cycle();
    if(max_duty_cycle == 0)
        *percentage = 0.f;
    else
    {
        *percentage = pic32_pwm5_get_duty_cycle() * 100.f;
        *percentage /= (float)max_duty_cycle;
    }

    return 0;
}

int pwm_set_frequency(uint8_t mikrobus_index, uint32_t frequency)
{
    return pwm_set_period(mikrobus_index, (1000 * 1000 * 1000) / frequency);
}

int pwm_set_period(uint8_t mikrobus_index, uint32_t period)
{
    float duty_cycle = 0.f;

    if (check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "pwm: Invalid mikrobus index\n");
        return -1;
    }
    pwm_get_duty_cycle(mikrobus_index, &duty_cycle);
    if (pic32_pwm5_init(period) != 0
    ||  pwm_set_duty_cycle(mikrobus_index, duty_cycle) != 0)
        return -1;

    if (enabled[mikrobus_index])
        return pwm_enable(mikrobus_index);

    return 0;
}

int pwm_get_period(uint8_t mikrobus_index, uint32_t *period)
{
    if (check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "pwm: Invalid mikrobus index\n");
        return -1;
    }
    if (period == NULL)
    {
        fprintf(stderr, "pwm: period cannot be null\n");
        return -1;
    }

    *period = pic32_pwm5_get_maximum_duty_cycle();

    return 0;
}

int pwm_get_frequency(uint8_t mikrobus_index, uint32_t *frequency)
{
    uint32_t max_duty_cycle;

    if (check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "pwm: Invalid mikrobus index\n");
        return -1;
    }
    if (frequency == NULL)
    {
        fprintf(stderr, "pwm: frequency cannot be null\n");
        return -1;
    }

    max_duty_cycle = pic32_pwm5_get_maximum_duty_cycle();
    if (max_duty_cycle == 0)
        return -1;

    /* Convert period in nanoseconds to frequency in Hz */
    *frequency = (1000 * 1000 * 1000) / max_duty_cycle;

    return 0;
}

int pwm_disable(uint8_t mikrobus_index)
{
    if (check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "pwm: Invalid mikrobus index\n");
        return -1;
    }

    if (pic32_pwm5_disable() != 0)
        return -1;

    enabled[mikrobus_index] = false;

    return 0;
}

int pwm_release(uint8_t mikrobus_index)
{
    if (check_valid_mikrobus(mikrobus_index) < 0)
    {
        fprintf(stderr, "pwm: Invalid mikrobus index\n");
        return -1;
    }

    return pic32_pwm5_release();
}

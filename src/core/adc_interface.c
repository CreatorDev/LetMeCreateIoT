#include "letmecreate/core/adc.h"

#include <stdio.h>
#include <pic32_gpio.h>

#include "letmecreate/core/common.h"

#define AD1_ENABLE 0x8000

#define AD1CON1_SSRC1 0x20
#define AD1CON1_SSRC2 0x40
#define AD1CON1_SSRC3 0x80
#define AD1CON1_PINS  0x10

#define AD1CON3_ALTS  0x01
#define AD1CON3_SAMC1 0x100
#define AD1CON3_SAMC2 0x200
#define AD1CON3_SAMC3 0x400
#define AD1CON3_SAMC4 0x800

static int enabled = 0;

static void adc_init()
{
    if(!enabled)
    {
        GPIO_CONFIGURE_AS_ANALOG(G, 9);
        GPIO_CONFIGURE_AS_INPUT(G, 9);

        AD1CON1CLR = AD1_ENABLE;

        AD1CON1 = AD1CON1_SSRC1 | AD1CON1_SSRC2 | AD1CON1_SSRC3;
        AD1CON2 = 0;
        AD1CON3 = AD1CON3_ALTS | AD1CON3_SAMC1 | AD1CON3_SAMC2 |
                  AD1CON3_SAMC3 | AD1CON3_SAMC4;

        AD1CON1SET = AD1_ENABLE;
        enabled = 1;
    }
}

static uint16_t read_analog(uint8_t pin)
{
    AD1CHS = pin << AD1CON1_PINS;
    AD1CON1bits.SAMP = 1;
    while( AD1CON1bits.SAMP || !AD1CON1bits.DONE );
    return ADC1BUF0;
}

int adc_get_value(uint8_t mikrobus_index, float * value)
{
    uint16_t tmp_val;

    adc_init();

    if(mikrobus_index != MIKROBUS_1)
    {
        fprintf(stderr, "ADC: Unknown Mikrobus index passed\n");
        return -1;
    }

    if(value == NULL)
    {
        fprintf(stderr, "ADC: Cannot write to null variable\n");
        return -1;
    }

    tmp_val = read_analog(19);

    *value = 5.0f * ((float)tmp_val / 1023.0f);
    return 0;
}

#include "letmecreate/core/power.h"

#include <stdio.h>
#include <contiki.h>

#define WDTCON_ON               0x8000
#define DEVCFG1_ENCLKSWITCHING  0x8000

#define SWITCH_PERIPHERAL(enum_value, bits) if(peripherals & (enum_value)) \
                                                  bits = value;

static void set_peripherals(int8_t value, uint64_t peripherals)
{
    SWITCH_PERIPHERAL(PERIPHERAL_AD, PMD1bits.AD1MD);
    SWITCH_PERIPHERAL(PERIPHERAL_CTMU, PMD1bits.CTMUMD);
    SWITCH_PERIPHERAL(PERIPHERAL_CVR, PMD1bits.CVRMD);
    SWITCH_PERIPHERAL(PERIPHERAL_CMP1, PMD2bits.CMP1MD);
    SWITCH_PERIPHERAL(PERIPHERAL_CMP2, PMD2bits.CMP2MD);
    SWITCH_PERIPHERAL(PERIPHERAL_IC1, PMD3bits.IC1MD);
    SWITCH_PERIPHERAL(PERIPHERAL_IC2, PMD3bits.IC2MD);
    SWITCH_PERIPHERAL(PERIPHERAL_IC3, PMD3bits.IC3MD);
    SWITCH_PERIPHERAL(PERIPHERAL_IC4, PMD3bits.IC4MD);
    SWITCH_PERIPHERAL(PERIPHERAL_IC5, PMD3bits.IC5MD);
    SWITCH_PERIPHERAL(PERIPHERAL_OC1, PMD3bits.OC1MD);
    SWITCH_PERIPHERAL(PERIPHERAL_OC2, PMD3bits.OC2MD);
    SWITCH_PERIPHERAL(PERIPHERAL_OC3, PMD3bits.OC3MD);
    SWITCH_PERIPHERAL(PERIPHERAL_OC4, PMD3bits.OC4MD);
    SWITCH_PERIPHERAL(PERIPHERAL_OC5, PMD3bits.OC5MD);
    SWITCH_PERIPHERAL(PERIPHERAL_T1, PMD4bits.T1MD);
    SWITCH_PERIPHERAL(PERIPHERAL_T2, PMD4bits.T2MD);
    SWITCH_PERIPHERAL(PERIPHERAL_T3, PMD4bits.T3MD);
    SWITCH_PERIPHERAL(PERIPHERAL_T4, PMD4bits.T4MD);
    SWITCH_PERIPHERAL(PERIPHERAL_T5, PMD4bits.T5MD);
    SWITCH_PERIPHERAL(PERIPHERAL_I2C1, PMD5bits.I2C1MD);
    SWITCH_PERIPHERAL(PERIPHERAL_I2C2, PMD5bits.I2C2MD);
    SWITCH_PERIPHERAL(PERIPHERAL_SPI1, PMD5bits.SPI1MD);
    SWITCH_PERIPHERAL(PERIPHERAL_SPI2, PMD5bits.SPI2MD);
    SWITCH_PERIPHERAL(PERIPHERAL_U1, PMD5bits.U1MD);
    SWITCH_PERIPHERAL(PERIPHERAL_U2, PMD5bits.U2MD);
    SWITCH_PERIPHERAL(PERIPHERAL_U3, PMD5bits.U3MD);
    SWITCH_PERIPHERAL(PERIPHERAL_U4, PMD5bits.U4MD);
//    SWITCH_PERIPHERAL(PERIPHERAL_USB, PMD5bits.USB1MD);
    SWITCH_PERIPHERAL(PERIPHERAL_PMP, PMD6bits.PMPMD);
    SWITCH_PERIPHERAL(PERIPHERAL_REFO, PMD6bits.REFOMD);
    SWITCH_PERIPHERAL(PERIPHERAL_RTC, PMD6bits.RTCCMD);
}

int power_enable_peripherals(uint64_t peripherals)
{
    set_peripherals(0, peripherals);

    return 0;
}

int power_disable_peripherals(uint64_t peripherals)
{
    set_peripherals(1, peripherals);

    return 0;
}

int power_select_system_clock(uint8_t clock)
{
    int32_t state;

    if(clock > SYSCLK_FIRCDIV)
    {
        fprintf(stderr, "Power: Invalid clock type\n");
        return -1;
    }

    if(DEVCFG1bits.FCKSM & DEVCFG1_ENCLKSWITCHING)
    {
        fprintf(stderr, "Power: Clock switching is disabled in DEVCFG1\n");
        return -1;
    }

    state = __builtin_get_isr_state();
    __builtin_disable_interrupts();

    SYSKEY = 0x0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    OSCCONbits.NOSC = clock;
    OSCCONbits.OSWEN = 1;

    while(OSCCONbits.OSWEN);

    SYSKEY = 0x0;

    __builtin_set_isr_state(state);

    return 0;
}

int power_set_watchdog(int8_t prescaler)
{
    if(prescaler > WDTTIMEOUT_1045876MS || prescaler < WDTTIMEOUT_DISABLE)
    {
        fprintf(stderr, "Power: Invalid timeout\n");
        return -1;
    }

    if(prescaler == WDTTIMEOUT_DISABLE)
    {
        WDTCONCLR = WDTCON_ON;
        return 0;
    }

    DEVCFG1bits.WDTPS = prescaler;
    WDTCONSET = WDTCON_ON;

    return 0;
}

int power_start_idle()
{
    OSCCONbits.SLPEN = 0;
    __asm__("WAIT");

    return 0;
}

int power_start_sleep()
{
    OSCCONbits.SLPEN = 1;
    __asm__("WAIT");

    return 0;
}

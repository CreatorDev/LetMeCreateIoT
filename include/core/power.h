/**
 * @file power.h
 * @author Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef H_CORE_POWERSAVING
#define H_CORE_POWERSAVING

#include <stdint.h>

/** @brief Peripherals
  *
  * List of all peripherals that can be enabled/disabled
 */
enum {
    /** All */
    PERIPHERAL_ALL  = 0x1FFFFFFFF,

    /** ADC */
    PERIPHERAL_AD   = 0x01,
    /** Charge Time Measurement Unit */
    PERIPHERAL_CTMU = 0x02,
    /** Comparator voltage reference */
    PERIPHERAL_CVR  = 0x04,

    PERIPHERAL_CMP1 = 0x08,
    PERIPHERAL_CMP2 = 0x10,
    /** Comparator */
    PERIPHERAL_CMP  = PERIPHERAL_CMP1 | PERIPHERAL_CMP2,

    PERIPHERAL_IC1  = 0x40,
    PERIPHERAL_IC2  = 0x80,
    PERIPHERAL_IC3  = 0x100,
    PERIPHERAL_IC4  = 0x200,
    PERIPHERAL_IC5  = 0x400,

    /** Input capture */
    PERIPHERAL_IC   = PERIPHERAL_IC1 | PERIPHERAL_IC2 | PERIPHERAL_IC3 |
                      PERIPHERAL_IC4 | PERIPHERAL_IC5,
    PERIPHERAL_OC1  = 0x800,
    PERIPHERAL_OC2  = 0x1000,
    PERIPHERAL_OC3  = 0x2000,
    PERIPHERAL_OC4  = 0x4000,
    PERIPHERAL_OC5  = 0x8000,
    /** Output compare */
    PERIPHERAL_OC   = PERIPHERAL_OC1 | PERIPHERAL_OC2 | PERIPHERAL_OC3 |
                      PERIPHERAL_OC4 | PERIPHERAL_OC5,
    PERIPHERAL_T1   = 0x10000,
    PERIPHERAL_T2   = 0x20000,
    PERIPHERAL_T3   = 0x40000,
    PERIPHERAL_T4   = 0x80000,
    PERIPHERAL_T5   = 0x100000,
    /** Timers */
    PERIPHERAL_T    = PERIPHERAL_T1 | PERIPHERAL_T2 | PERIPHERAL_T3 |
                      PERIPHERAL_T4 | PERIPHERAL_T5,

    PERIPHERAL_I2C1 = 0x200000,
    PERIPHERAL_I2C2 = 0x400000,
    /** I2C buses */
    PERIPHERAL_I2C  = PERIPHERAL_I2C1 | PERIPHERAL_I2C2,

    PERIPHERAL_SPI1 = 0x800000,
    PERIPHERAL_SPI2 = 0x1000000,
    /** SPI Buses */
    PERIPHERAL_SPI  = PERIPHERAL_SPI1 | PERIPHERAL_SPI2,

    PERIPHERAL_U1   = 0x2000000,
    PERIPHERAL_U2   = 0x4000000,
    PERIPHERAL_U3   = 0x8000000,
    PERIPHERAL_U4   = 0x10000000,
    /** UART */
    PERIPHERAL_U    = PERIPHERAL_U1 | PERIPHERAL_U2 | PERIPHERAL_U3 |
                      PERIPHERAL_U4,

    //PERIPHERAL_USB  = 0x20000000,
    /** Parallel master port */
    PERIPHERAL_PMP  = 0x40000000,
    /** Real time clock */
    PERIPHERAL_RTC  = 0x80000000,
    /** Reference Oscillator Module */
    PERIPHERAL_REFO = 0x100000000,
};

enum {
/** Fast Internal RC Oscillator (FRC) */
    SYSCLK_FRC          = 0x00,
/** Fast Internal RC Oscillator with PLL module via Postscaler */
    SYSCLK_FRCPLL       = 0x01,
/** Primary Oscillator */
    SYSCLK_PRIMARY      = 0x02,
/** Primary Oscillator with PLL module */
    SYSCLK_PRIMARY_PLL  = 0x03,
/** Secondary Oscillator */
    SYSCLK_SECONDARY    = 0x04,
/** Low Power Internal RC Oscillator */
    SYSCLK_LPRC         = 0x05,
/** Fast Internal RC Oscillator divided by 16 */
    SYSCLK_FIRCDIV16    = 0x06,
/** Fast Internal RC Oscillator divided by OSCCON */
    SYSCLK_FIRCDIV      = 0x07,
};

/** @brief Enable selected peripherals
  *
  *
  * @param[in] peripherals Bitmask of peripherals to enable
  * @return 0 if successful, -1 if failed
  */
int power_enable_peripherals(uint64_t peripherals);

/** @brief Disable selected peripherals
  *
  * @param[in] peripherals Bitmask of peripherals to disable
  * @return 0 if successful, -1 if failed
  */
int power_disable_peripherals(uint64_t peripherals);

/** @brief Change system clock source
  *
  * @param[in] clock Clock which will be used
  * @return 0 if succesful, -1 if failed
  */
int power_select_system_clock(uint8_t clock);

/** @brief Enables/disables the wakeup watchdog
  *
  * @param[in] enable Pass 1 to enable, 0 to disable
  * @return 0 if successful, -1 if failed
  */
int power_set_watchdog(uint8_t enable);

/** @brief Puts the device in idle mode
  *
  * To wakeup from idle after a delay use #power_set_watchdog
  *
  * @return 0 if successful, -1 if failed to enter idle mode
  */
int power_start_idle();

/** @brief Puts the device in sleep mode
  *
  * To wakeup from sleep after a delay use #power_set_watchdog
  *
  * @return 0 if successful, -1 if failed to enter sleep mode
  */
int power_start_sleep();


#endif

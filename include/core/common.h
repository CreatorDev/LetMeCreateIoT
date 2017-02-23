/**
 * @file include/core/common.h
 * @author Francois Berder, Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef H_CORE_COMMON
#define H_CORE_COMMON

#include <stdint.h>

/** Index of Mikrobus interfaces */
enum MIKROBUS_INDEX {
    MIKROBUS_1,
    MIKROBUS_COUNT
};


/* Disables timeouts */
#define NO_TIMEOUT (0)

/**
 * @brief Checks if mikrobus exists
 *
 * @param[in] mikrobus_index Mikrobus to check
 * @return 0 if successful, -1 otherwise
 */
int check_valid_mikrobus(uint8_t mikrobus_index);

#endif

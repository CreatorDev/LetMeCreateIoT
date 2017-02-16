#include "letmecreate/core/common.h"

int check_valid_mikrobus(uint8_t mikrobus_index)
{
    if(mikrobus_index >= MIKROBUS_COUNT)
        return -1;

    return 0;
}

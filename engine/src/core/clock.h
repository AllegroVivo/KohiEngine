#pragma once

#include "defines.h"

typedef struct clock
{
    Double start_time;
    Double elapsed;
} clock;

KAPI void clock_update(clock* clock);

KAPI void clock_start(clock* clock);

KAPI void clock_stop(clock* clock);
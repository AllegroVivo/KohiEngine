#pragma once

#include "defines.h"

typedef struct clock
{
    Double start_time;
    Double elapsed;
} clock;

void clock_update(clock* clock);
void clock_start(clock* clock);
void clock_stop(clock* clock);
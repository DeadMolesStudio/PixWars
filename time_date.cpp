#include "time_date.h"

time_t get_block_time(time_t time)
{
    time += 600000000;
    return time;
}

time_t get_difference(time_t t)
{
    time_t now;
    time(&now);

    time_t utc_now =  now;

    if ( utc_now >= t )
        return time(NULL);
    return t - utc_now;
}



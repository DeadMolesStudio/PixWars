//
// Created by Ksenia Kazantseva on 06.04.2018.
//

#include "time_date.h"

void print_time(time_t time)
{
    struct tm * timeinfo;
    char str_time [18];
    timeinfo = localtime(&time);
    strftime(str_time,18,"%x %X", timeinfo);
    std::cout << str_time << std::endl;
}

std::string get_time(time_t time)
{
    struct tm * timeinfo;
    char str_time [18];
    timeinfo = localtime(&time);
    strftime(str_time,18,"%x %X", timeinfo);

    return str_time;
}

time_t get_block_time(time_t time)
{
    time += 60 * 5;
    return time;
}

time_t get_difference(time_t t)
{
    time_t now;
    time(&now);

    if ( now > t )
        return time(NULL);
    return t - now;
}



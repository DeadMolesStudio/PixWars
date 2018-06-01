#ifndef TIME_DATE_H
#define TIME_DATE_H

#include <iostream>
#include <string.h>
#include <ctime>

//прибавляет 1 минуту ко времени
time_t get_block_time(time_t time);
//возвращает разницу с текущем временем в секундах
time_t get_difference(time_t t);

#endif //TIME_DATE_H

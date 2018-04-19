#ifndef TIME_DATE_H
#define TIME_DATE_H

#include <iostream>
#include <string.h>
#include <ctime>

// печатает дату и время
void print_time(time_t time);
//возвращает дату и время в строковом формате "mm/dd/yy hh:mm::ss"
std::string get_time(time_t time);
//при,авляет 5 минут ко времени
time_t get_block_time(time_t time);
//возвращает разницу с текущем временем в секундах
time_t get_difference(time_t t);

#endif //TIME_DATE_H

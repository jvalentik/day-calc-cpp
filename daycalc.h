//daycalc.h - header for day calc functions 21-05-09 19:08
#ifndef DAYCALC_H_
#define DAYCALC_H_

#include "tools.h"

void count_from_today(int d=0, int m=0, int y=0) 
                      throw(PWJ::bad_input, std::bad_alloc);
void count_from_date(int d1=0, int m1=0, int y1=0,
                     int d2=0, int m2=0, int y2=0)
                     throw(PWJ::bad_input, std::bad_alloc);
void count_from_file(void) throw(PWJ::bad_input, std::bad_alloc);
void show_help(void) throw(PWJ::bad_file);
void display_today(void) throw (std::bad_alloc);
void check_leap_year(int y=0) throw(PWJ::bad_input, std::bad_alloc);
void display_name(int d=0, int m=0, const char * n="") 
                  throw(PWJ::bad_input, std::bad_alloc);

#endif //DAYCALC_H_

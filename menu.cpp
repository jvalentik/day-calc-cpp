//menu.cpp - Implementation of menu and other support funcs 23-05-09 21:12 
#include "daycalc.h"
#include <cctype>
#include <cstring>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

namespace {
   enum  {
      actual_date = 'A',
      enter_date = 'Z',
      load_date = 'N',
      show_info = 'I',
      show_date = 'D',
      exit_program = 'U',
      leap_year = 'P',
      show_name = 'M'
   };
   
   const char * const CHOICES("ADIPUMNZ");                 
} //end of unnamed namespace               

void display_menu() throw() {
   cout << "\n\n\t\t***MENU***\n\n" 
        << '\t' << char(actual_date) << "\tPocitat od aktualneho datumu\n"
        << '\t' << char(enter_date) << "\tZadaj datumy na vypocet\n"
        << '\t' << char(load_date) << "\tNacitat datumy zo suboru\n"
        << '\t' << char(show_date) << "\tZobrazit dnesny datum a cas\n"
        << '\t' << char(show_name) << "\tMeniny\n"
        << '\t' << char(leap_year) << "\tZistit priestupny rok\n"       
        << '\t' << char(show_info) << "\tZobrazit info o programe\n"
        << '\t' << char(exit_program) << "\tUkoncit program\n\n";
}

char get_choice() throw() {
   cout << "Akcia: ";
   char choice;
   while ((cin >> choice).get() && 
           std::strchr(CHOICES, std::toupper(choice)) == NULL) {
      cout << "\n" << choice << " nie je platna volba!\n";   
      cout << "Akcia: ";
   }
   return std::toupper(choice);
}

bool do_choice(char choice) throw() {
   bool continue_run(true);
   switch(choice) {
      case actual_date:
         count_from_today();
         break;
      case enter_date:
         count_from_date();
         break;
      case load_date:
         count_from_file();
         break;
      case show_info:
         show_help();
         break;
      case show_date:
         display_today();
         break; 
      case leap_year:
         check_leap_year();
         break;
      case show_name:
         display_name();
         break;      
      case exit_program:
         continue_run = false;
         break;      
   }
   return continue_run;
} 

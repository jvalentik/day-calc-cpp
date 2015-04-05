/*Name: DayCalc 3.0 SK 
  Copyright: GPL open source
  Author: PowerJ Slovakia
  Date: 115-06-09 22:00
  Description: Day calculator written in Dev-C++ for studying purposes
*/
#include "calendar.h"
#include "daycalc.h"
#include "menu.h"
#include "tools.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

using PWJ::Calendar;
using PWJ::convert;

int main(int argc, char * argv[]) {
   if (argc == 1) { // no command line commands
      try {
         char choice;
         do {
            display_menu();           
            choice = get_choice();
         } while(do_choice(choice));
      }
      catch (Calendar::invalid_date & p) {
         p.report();
         std::exit(EXIT_FAILURE);
      }     
      catch (std::bad_alloc & ba) {
         std::cerr << "Nedostatok pamate!\n"
                   << ba.what() << "\n";
         std::abort();
      }
      catch (std::exception & e) {
         std::cerr << e.what();
         std::exit(EXIT_FAILURE);
      }   
      catch (...) {
         std::cerr << "\n***Doslo k neocakavanej chybe!***\n";
         std::exit(EXIT_FAILURE);
      } 
   }
   else { // command line support
      if (std::strcmp(argv[1], "-h") == 0) {
         show_help();
      }
      else if (std::strcmp(argv[1], "-t") == 0) {
         if (argc == 2) {
            display_today();
         }
         else if (argc == 5) {
            int d, m, y;
            try {
               convert(argv[2], &d);
               convert(argv[3], &m);
               convert(argv[4], &y);               
            }
            catch (PWJ::bad_input & bi) {
               std::cerr << bi.what();
               std::exit(EXIT_FAILURE);
            }
            try {
               count_from_today(d, m, y);
            }
            catch (Calendar::invalid_date & id) {
               id.report();
               std::exit(EXIT_FAILURE);
            }
         }
         else {
            std::cerr << "Nespravne hodnoty pre parameter -t!\n"
                      << "Zadaj " << argv[0] << " -h pre napovedu.\n";
            return EXIT_FAILURE;
         }
      }
      else if (std::strcmp(argv[1], "-d") == 0) {
         if (argc == 8) {
            int d1, m1, y1, d2, m2, y2;
            try {
               convert(argv[2], &d1);
               convert(argv[3], &m1);
               convert(argv[4], &y1);
               convert(argv[5], &d2);
               convert(argv[6], &m2);
               convert(argv[7], &y2);
            }
            catch (PWJ::bad_input & bi) {
               std::cerr << bi.what();
               std::exit(EXIT_FAILURE);
            }
            try {
               count_from_date(d1, m1, y1, d2, m2, y2);
            }
            catch (Calendar::invalid_date & id) {
               id.report();
               std::exit(EXIT_FAILURE);
            }
         }
         else {
            std::cerr << "Nespravne hodnoty pre parameter -d!\n"
                      << "Zadaj " << argv[0] << " -h pre napovedu.\n";
            return EXIT_FAILURE;
         }
      }
      else if (std::strcmp(argv[1], "-n") == 0) {
         if (argc == 3) {
            display_name(0,0,argv[2]);
         }
         else if (argc == 4) {
            int d, m;
            try {
               convert(argv[2], &d);
               convert(argv[3], &m);
            }
            catch (PWJ::bad_input & bi) {
               std::cerr << bi.what();
               std::exit(EXIT_FAILURE);
            }
            display_name(d, m);
         }
         else {
            std::cerr << "Nespravne hodnoty pre parameter -n!\n"
                      << "Zadaj " << argv[0] << " -h pre napovedu.\n";
            return EXIT_FAILURE;
         }            
      }
      else if (std::strcmp(argv[1], "-y") == 0) {
         if (argc == 3) {
            int y;
            try {
               convert(argv[2], &y);
            }
            catch (PWJ::bad_input & bi) {
               std::cerr << bi.what();
               std::exit(EXIT_FAILURE);
            }
            check_leap_year(y);
         }
         else {
            std::cerr << "Nespravne hodnoty pre parameter -y!\n"
                      << "Zadaj " << argv[0] << " -h pre napovedu.\n";
            return EXIT_FAILURE;
         }            
      }
      else {
         std::cerr << argv[1] << " nie je platny parameter!\n"
                   << "Zadaj " << argv[0] << " -h pre napovedu.\n";
         return EXIT_FAILURE;
      }         
   }         
   return EXIT_SUCCESS;
}

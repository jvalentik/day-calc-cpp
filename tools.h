//tools.h - supporting templates and funcs for DayCalc 23-05-09 21:11
#ifndef TOOLS_H_
#define TOOLS_H_

#include <cstring>
#include <cctype>
#include <exception>
#include <iostream>
#include <string>

namespace PWJ {
   namespace {
      int max_tries = 3;
   }
   
   inline void resetTries(int n) {
      if (n >= 0) {
         max_tries = n;
      }
   }
   
   class bad_input : public std::exception {
      std::string msg;
      public:
         explicit bad_input(const char * s = "Neplatne data!\n") throw() 
                           : std::exception(), msg(s) {}
         virtual ~bad_input() throw () {}
         virtual const char * what()const throw() {return msg.c_str();}
         virtual void report() const throw(); 
   };
   
   inline void bad_input::report() const throw() { 
      std::cout << "Pokus o nacitanie poskodenych dat vo funkcii "
                << msg << "\n";     
   }
   
   class bad_file : public bad_input {   
      public:
         bad_file(const std::string & n) : bad_input(n.c_str()) {}
         bad_file(const char * s = "neznamy subor") : bad_input(s) {}
         ~bad_file() throw() {}
         const char * what() const throw() { return bad_input::what();}
         void report() const throw();
   };
   
   inline void bad_file::report() const throw() {
      std::cout << "Nepodarilo sa otvorit subor " << what() << ".\n";
   }
         
   template <typename T>
   T read(const char * prompt = ":" ) throw(bad_input) {
      T value;
      int tried(0);
      std::cout << prompt;      
      while (!(std::cin >> value)) {         
         tried++;
         std::cerr << "Neplatne vstupne data!\n";
         std::cin.clear();
         while (std::cin.get() != '\n')
            continue;
         if (tried >= max_tries) throw bad_input("Neplatne udaje");
         std::cerr << "Opakuj zadanie: ";
      }
      while (std::cin.get() != '\n')
         continue;
      return value;
   }
   
   template <typename T>
   T read(std::istream & is) throw(bad_input) {
      T value;
      if (!(is >> value)) {
         is.clear();
         while (is.get() != '\n')
            continue;
         throw bad_input("read<Date>");
      }
      return value;
   }   
   
   template <typename T> 
   void convert(char * c, T * value) throw(PWJ::bad_input) {            
      int i(0);
      int ones, tens, hundreds, thousands;  
      ones = tens = hundreds = thousands = 0;    
      while (i < std::strlen(c)) {
         if (!(std::isdigit(c[i]))) {
            std::cerr << "Neplatny znak!\n";         
            throw PWJ::bad_input("convert");   
         }
         i++;
      }
      int digits = std::strlen(c);
      switch (digits) {
         case 1:
            ones = int(c[0]) - 48;
            break;
         case 2:
            tens = (int(c[0]) - 48) * 10;
            ones = int(c[1]) - 48;
            break;
         case 3:
            hundreds = (int(c[0]) - 48) * 100;
            tens = (int(c[1]) - 48) * 10;
            ones = int(c[2]) - 48;
            break;
         case 4:
            thousands = (int(c[0]) - 48) * 1000;
            hundreds = (int(c[1]) - 48) * 100;
            tens = (int(c[2]) - 48) * 10;
            ones = int(c[3]) - 48;
            break;
         default:
            std::cerr << "Prilis vela parametrov!\n";
            throw PWJ::bad_input("read<int>(const char *)");
      }         
      *value = thousands + hundreds + tens + ones;
}     
   
   inline void txtClear() { system("CLS"); } // system dependent Windows   
   //inline void txtClear() { system("clear"); } // system dependent Linux
   
} // namespace PWJ ends

#endif //TOOLS_H_         

//daycalc.cpp - implementation of Day Calc Functions 21-05-09 18:41
#include "calendar.h"
#include "tools.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

using PWJ::Calendar;
using PWJ::Date;
using PWJ::CurrentDate;
using PWJ::CurrentTime;
using PWJ::DateTime;
using PWJ::read;
using PWJ::bad_input;
using PWJ::bad_file;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::stringstream;

namespace {
   const char * const SM_CHOICES("UE");
   
   void show_result(const Date & aDate, const Date & bDate, std::ostream & os) {
      unsigned years((aDate - bDate) / 365);   
      unsigned months(((aDate - bDate) % 365) / 30);
      unsigned days_left(((aDate - bDate) % 365) % 30);
      PWJ::txtClear();
      os << "Medzi datumom " << aDate << "\na datumom " << bDate 
         << " uplynie " << aDate - bDate << " dni.\n";
      os << "Celkovo je to " << years << " rokov " << months
         << " mesiacov" << " a " << days_left << " dni.\n"; 
   }
   
   void show_submenu() {
      cout << "\nStlac U ak chces vysledok ulozit,\n"
           << "E ak chces vysledok exprotovat do TXT,\n"
           << "alebo lubovolny klaves pre pokracovanie: ";
   }
   
   char get_sm_choice() {      
      char choice;
      cin.get(choice);
      return std::toupper(choice);
   } 
   
   void save_file(Date & aDate, Date & bDate, const string & fn)
        throw(std::bad_alloc) {      
      ofstream fout(fn.c_str(), std::ios_base::app | std::ios_base::binary);
      try {
         if (!fout.is_open())
            throw bad_file(fn);
      }
      catch (bad_file & bf) {
         bf.report();
         cout << "Data neulozene!\n";         
         cin.get();
         return;         
      }
      CurrentDate * p_Cdate = new CurrentDate;
      CurrentTime * p_Ctime = new CurrentTime;   
      DateTime * p_Dt = new DateTime(*p_Cdate, *p_Ctime);
      delete p_Cdate;
      delete p_Ctime;
      p_Cdate = 0;
      p_Ctime = 0;         
      p_Dt->WriteAll(fout);         
      aDate.WriteAll(fout);
      bDate.WriteAll(fout);
      delete p_Dt;                                          
   }
   
   void export_file(const Date & aDate, const Date & bDate,
                    const string & fn) throw() {
      try {      
         ofstream out_file(fn.c_str());                  
         if(!out_file.is_open()) {
            throw bad_file(fn);
         }        
         show_result(aDate, bDate, out_file);
      }      
      catch(bad_file & p) {
         p.report();
         cout << "Data neulozene!\n";
         cin.get();
         return;         
      }      
   }
   
   class ShowDateTime {
      private:
         mutable int m_Count;
      public:
         explicit ShowDateTime(int i) : m_Count(i) {}
         void operator()(const Calendar * p_C) const;
   };
   
   void ShowDateTime::operator()(const Calendar * p_C) const {                            
      if (typeid(DateTime) ==  typeid(*p_C)) {         
         cout << std::right << std::setw(5) << m_Count++ 
              << ":" << std::setw(10) << *p_C << endl;
      }
   }
   
   class Store {
      private:
         std::ostream & m_Os;
      public:
         explicit Store(std::ostream & os) : m_Os(os) {}
         void operator()(const Calendar * p_C) const;
   };  
   
   void Store::operator()(const Calendar * p_C) const {
      p_C->WriteAll(m_Os);
   }
   
   void freeMemory(const Calendar * p_C) {
      delete p_C;
   }
   
   bool load_file(Date & aDate, Date & bDate, const string & fn) 
                                           throw(std::bad_alloc) {
      std::fstream finout(fn.c_str(), std::ios_base::in | std::ios_base::out
                         | std::ios_base::binary); 
      try {       
         if (!finout.is_open()) {
            throw bad_file(fn);
         }        
      }
      catch (bad_input & p) {
         p.report();               
         return false;
      }   
      std::vector<const Calendar *> dates;             
      tm temp;             
      while (finout) {
         finout.read((char *) &temp, sizeof(tm));
         if (!finout) break;
         dates.push_back(new DateTime(&temp));
         finout.read((char *) &temp, sizeof(tm));         
         dates.push_back(new Date(&temp));
         finout.read((char *) &temp, sizeof(tm));         
         dates.push_back(new Date(&temp));         
      }
      if (!finout.eof()) {
         finout.close();   
         for_each(dates.begin(), dates.end(), freeMemory);
         dates.clear();               
         return false;
      }
      finout.close();
      finout.clear();
      if (dates.empty()) {
         cout << "Subor neobsahuje ziadne data!\n";
         return false;
      }
      else {           
         cout << "Cislo:" << std::right
              << std::setw(20) << "Datum ulozenia" << endl;
         for_each(dates.begin(), dates.end(), ShowDateTime(1));         
      }
      int choice;
      char ch;
      while (true) {
         choice = read<int>("Zadaj cislo zaznamu: ");
         if (choice < 1 || choice > (dates.size() / 3)) {
            cout << "Neplatna volba!\n";
            continue;
         }         
         cout << "Prajes si zvoleny zaznam nacitat alebo odstranit? <N/O>: ";         
         while (cin.get(ch) && (toupper(ch) != 'N' && toupper(ch) != 'O')) {
            cout << "Zadaj N alebo O!\n";
         }         
         break;
      }
      int i, j, k;
      for (i = 1, j = 1, k = 2; k <= dates.size(); i++, j+=3, k+=3) {
         if (i == choice && toupper(ch) == 'N') {               
            aDate = *(dynamic_cast<const Date *> (dates[j]));
            bDate = *(dynamic_cast<const Date *> (dates[k]));
            break;
         }
         else if (i == choice && toupper(ch) == 'O') {
            const int POS((choice - 1) * 3);
            i = 0;  
            std::vector<const Calendar *>::iterator iter(dates.begin());               
            while (i++ != POS) {
               iter++;
            }
            for (i = 0; i < 3; i++) {
               if (i == 1) { // first date after DateTime
                  aDate = *(dynamic_cast<const Date *> (*iter));
               }
               if (i == 2) { //second date after DateTime
                  bDate = *(dynamic_cast<const Date *> (*iter));
               }                   
               delete *iter;
               iter = dates.erase(iter);
               
            }      
            finout.open(fn.c_str(), std::ios_base::out | 
                        std::ios_base::trunc | std::ios_base::binary);
            for_each(dates.begin(), dates.end(), Store(finout));
            finout.close();                       
         }                                       
      }       
      for_each(dates.begin(), dates.end(), freeMemory);       
      return true;     
   }
   
   const Date * const get_name(const string & d) 
                 throw(bad_file, std::bad_alloc) {
      const int DAYS_PER_YEAR(365);  
      std::vector<Date *> dates(DAYS_PER_YEAR);
      dates[0] = new Date(1, 1, 2009);
      register int i;
      for (i = 1; i < DAYS_PER_YEAR; i++) {
         dates[i] = new Date(++(*dates[i-1]));
      }
      typedef std::pair<const string, Date *> Pair;
      std::map<string, Date *> names;
      ifstream fin;
      fin.open("names.txt");
      if (!fin.is_open()) {
         for_each(dates.begin(), dates.end(), freeMemory);         
         throw bad_file("names.txt");
      }
      i = 0;
      string name;
      getline(fin, name);
      while (fin) {
         names.insert(Pair(name, dates[i++]));
         getline(fin, name);
      }
      if (!fin.eof()) {
         for_each(dates.begin(), dates.end(), freeMemory);
         throw bad_file("names.txt");
      }
      fin.close();
      Date * p_result = 0;
      std::map<const string, Date *>::iterator it;
      for (it = names.begin(); it != names.end(); it++) {
         if (it->first.find(d) != string::npos) {
            p_result = new Date(*(it->second));   
            break;
         }
      }
      for_each(dates.begin(), dates.end(), freeMemory);                       
      return p_result;
   }
   
   string get_name(const Date & d) throw(bad_file, std::bad_alloc) {         
      const int DAYS_PER_YEAR(365);
      typedef std::pair<const Date, string> Pair;
      std::vector<Date *> dates(DAYS_PER_YEAR);
      dates[0] = new Date(1, 1, 2009);
      register int i;
      for (i = 1; i < DAYS_PER_YEAR; i++) {
         dates[i] = new Date(++(*dates[i-1]));
      }
      std::map<Date, string> names;
      ifstream fin;      
      fin.open("names.txt");
      if (!fin.is_open()) {
         for_each(dates.begin(), dates.end(), freeMemory);
         throw bad_file("names.txt");
      }
      i = 0;
      string name;
      getline(fin, name);
      while (fin) {         
         names.insert(Pair(*dates[i++], name));
         getline(fin, name);
      }
      if (!fin.eof()) {
         for_each(dates.begin(), dates.end(), freeMemory);
         throw bad_file("names.txt");
      }
      fin.close();
      for_each(dates.begin(), dates.end(), freeMemory);      
      return names[d];
  }       
} // end of unnamed namespace

void count_from_today(int d, int m, int y) throw(bad_input, std::bad_alloc) {
   std::auto_ptr<CurrentDate> aDate(new CurrentDate);
   std::auto_ptr<Date> bDate(new Date);        
   if (d == 0 && m == 0 && y == 0) {      
      while (true) {
         try {
            cin >> *bDate;
         }
         catch (Calendar::invalid_date & p) {
            p.report();
            continue;
         }   
         break;
      }
  }
  else {
     try {
      *bDate = Date(d, m, y);
     }
     catch (Calendar::invalid_date & p) {
        p.report();
        return;
     }
  }     
  show_result(*aDate, *bDate, cout);
  show_submenu();
  const char sm_choice(get_sm_choice());
  if (sm_choice == 'U') {
     string filename(read<string>("Zadaj nazov suboru: "));
     filename += ".pwj";
     save_file(*aDate, *bDate, filename);
  }
  if (sm_choice == 'E') {
     string filename(read<string>("Zadaj nazov suboru: "));
     filename += ".txt";
     export_file(*aDate, *bDate, filename); 
  }
}

void count_from_date(int d1, int m1, int y1,
                     int d2, int m2, int y2) throw(bad_input) {
   std::auto_ptr<Date> aDate(new Date);
   std::auto_ptr<Date> bDate(new Date);   
   
   if (d1 == 0 && d2 == 0) {
      while (true) {
         try {
            cout << "Zadaj prvy datum:\n";
            cin >> *aDate;
            cout << "Zadaj druhy datum:\n";
            cin >> *bDate;
         }
         catch (Calendar::invalid_date & p) {
            p.report();
            continue;
         }
         break;
      }
   }
   else {
      try {
         *aDate = Date(d1, m1, y1);
         *bDate = Date(d2, m2, y2);
      }
      catch (Calendar::invalid_date & p) {
         p.report();
         return;
      } 
   }      
   show_result(*aDate, *bDate, cout);
   show_submenu();
   const char sm_choice(get_sm_choice());
   if(sm_choice == 'U') {
      string filename(read<string>("Zadaj nazov suboru: "));
      filename += ".pwj";
      save_file(*aDate, *bDate, filename);
   }
   if(sm_choice == 'E') {
      string filename(read<string>("Zadaj nazov suboru: "));
      filename += ".txt";
      export_file(*aDate, *bDate, filename); 
   }  
}

void count_from_file() throw(bad_input, std::bad_alloc) {
   std::auto_ptr<Date> aDate(new Date);
   std::auto_ptr<Date> bDate(new Date); 
   string filename(read<string>("Zadaj nazov suboru: "));
   filename += ".pwj";
   if (load_file(*aDate, *bDate, filename)) {
      show_result(*aDate, *bDate, cout);
   }
   else {
      cout << "\nUdaje nenacitane!\n";
   }      
} 

void show_help() throw(bad_file) {
   stringstream storage;
   ifstream fin("daycalc.hlp");   
   if(!fin.is_open()) 
      throw bad_file("daycalc.hlp");
   string line; 
   getline(fin, line);
   while(fin) {      
      storage << line << '\n';
      getline(fin, line);           
   }
   if (fin.fail() && !fin.eof())
      throw bad_file("daycalc.hlp");
   PWJ::txtClear();           
   cout << storage.str();
   cout << "\nPokracuj stlacenim ENTER...";
   cin.get();  
}

void display_today() throw(std::bad_alloc) {
   Calendar * p_Today;
   p_Today = new CurrentDate;
   cout << "\nDnes je " << *p_Today << ", meniny ma ";
   string name("");
   try {
      name = get_name(*(dynamic_cast<Date *> (p_Today)));
   }
   catch (bad_file & bf) {
      bf.report();
   }
   cout << name << endl;
   delete p_Today;
   p_Today = new CurrentTime;
   cout << "\nJe " << *p_Today << " hodin.\n";
   delete p_Today;      
}    
   
void check_leap_year(int year) throw(bad_input) {   
   std::auto_ptr<Date> aDate(new Date);    
   if (year != 0) {
      try {
         *aDate = Date(1, 1, year);
      }
      catch (Calendar::invalid_date & p) {
         p.report();
         return;
      } 
   }
   else {
      while (true) {
         year = read<int>("Zadaj rok ktory chces overit: ");
         try {
            *aDate = Date(1, 1, year);
         }
         catch (Calendar::invalid_date & p) {
            p.report();
            continue;
         }      
         break;
      }
   }
   cout << "Rok " << year << " je " 
        << (aDate->GetLeap() == true ? "priestupny" : "nepriestupny") << ".\n";     
}

void display_name(int day, int mon, const char * n) 
                   throw(bad_input, std::bad_alloc) {
   string name(n);
   if (day == 0 && mon == 0 && name.empty()) {
      cout << "Zadaj datum <den mesiac>, alebo meno: ";
      cin >> day >> mon;
      if (cin.fail()) {
         cin.clear();
         getline(cin, name);
      }
      else {
         while (cin.get() != '\n')
            continue;
      }
   }   
   const Date * p_temp = 0;
   if (name.empty()) {      
      while (true) {            
         try {
            p_temp = new Date(day, mon, 2009);
         }
         catch (Calendar::invalid_date & id) {
            id.report();            
            day = read<int>("Zadaj den: ");
            mon = read<int>("Zadaj mesiac: ");
            continue;
         }
         break;
      }      
      try {
         name = get_name(*p_temp);
      }
      catch (bad_file & bf) {
         bf.report();
         delete p_temp;
         return;
      }
      Date * pd = const_cast<Date *> (p_temp);
      pd->Setf(Calendar::short_no_year);
      pd = 0;
      cout << *p_temp << " ma meniny " << name << endl;
      delete p_temp;
   }
   else {       
      try {
         p_temp = get_name(name);         
      }
      catch (bad_file & bf) {
         bf.report();
         return;
      }
      if (p_temp) {
         name = get_name(*p_temp); // in order to complete shortcuts of name
         Date * pd = const_cast<Date *> (p_temp);
         pd->Setf(Calendar::short_no_year);
         pd = 0;
         cout << name << " ma meniny " << *p_temp << endl;
         delete p_temp;
      }
      else {
         cout << name << " sa nenachadza v slovenskom kalendari!\n";
      }
   }  
}

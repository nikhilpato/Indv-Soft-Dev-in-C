#include "Dialog.h"

void Dialog::ReadLimitInt(std::istream &is, std::ostream &os, 
            int *val, int lo, int hi, std::string prompt) {
      while (true) {
         os << prompt << " [" << lo << ", " << hi << "]: ";

         is >> *val;
         if (is.fail()) {
            os << "Badly formatted input" <<std::endl;
         } else if (is.peek() != EOF && is.peek() != '\n') {
            os << "Unexpected garbage after value." <<std::endl;
         } else if (lo > *val || *val > hi) {
            os << "Please enter a value between " << lo << " and " << hi 
             <<std::endl;
         } else {
            break;
         }
         is.clear();
            is.ignore(256,'\n');
      }

      is.clear();
      is.ignore(256,'\n');
   };
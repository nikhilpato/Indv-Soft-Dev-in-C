#ifndef MANCALADLG_H
#define MANCALADLG_H

#include "Class.h"
#include "Dialog.h"

class MancalaDlg : public Dialog {
public:
   bool Run(std::istream &in, std::ostream &out, void *data);
   const Class *GetClass() const { return &mClass;};
   const static Class mClass;

   static Object *Create() {return new MancalaDlg();}
};

#endif
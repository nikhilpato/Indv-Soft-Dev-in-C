#ifndef OTHELLODLG_H
#define OTHELLODLG_H

#include "Class.h"
#include "Dialog.h"

class OthelloDlg : public Dialog {
public:
   bool Run(std::istream &in, std::ostream &out, void *data);
   const Class *GetClass() const {return &mClass;};
   const static Class mClass;

   static Object *Create() {return new OthelloDlg();}
};

#endif

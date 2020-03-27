#ifndef CHECKERSVIEW_H
#define CHECKERSVIEW_H

#include "View.h"

class CheckersBoard;

class CheckersView : public View {
public:
   void Draw(std::ostream &out);
   const static Class mClass;
   const Class *GetClass() const { return &mClass;};
   
private:
   static Object *Create() {return new CheckersView();}
};

#endif

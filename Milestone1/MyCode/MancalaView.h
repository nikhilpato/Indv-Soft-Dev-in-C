#ifndef MANACALAVIEW_H
#define MANCALAIEW_H

#include "View.h"

class MancalaBoard;

class MancalaView : public View {
public:
   void Draw(std::ostream &out);
   const static Class mClass;
   const Class *GetClass() const { return &mClass;};

private:
   static Object *Create() {return new MancalaView();}
};

#endif
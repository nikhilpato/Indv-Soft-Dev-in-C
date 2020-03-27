#ifndef OTHELLOVIEW_H
#define OTHELLOVIEW_H

#include "View.h"

class OthelloBoard;

class OthelloView : public View {
public:
   void Draw(std::ostream &out);
   const static Class mClass;
   const Class *GetClass() const { return &mClass;};
   
private:
   static Object *Create() {return new OthelloView();}
};

#endif

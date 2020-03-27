#ifndef CHECKERSVIEW_H
#define CHECKERSVIEW_H

#include "View.h"

class CheckersView : public View {
public:
   void Draw(std::ostream &out);
   
private:
   static Object *Create() {return new CheckersView();}
};

#endif

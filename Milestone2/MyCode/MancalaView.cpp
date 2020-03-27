#include "MancalaView.h"
#include "MancalaBoard.h"
#include <string>

using namespace std;

const Class MancalaView::mClass("MancalaView", Create);

void MancalaView::Draw(ostream &out) {
   string boardVals;
   string temp;
   int counter;
   char sqr;
   shared_ptr<const MancalaBoard> ob
    = dynamic_pointer_cast<const MancalaBoard>(mModel);

   out << "Black  12  11  10   9   8   7" << endl;
   out << "    -----------------------------" << endl;
   for (counter = 12; counter > 6; counter--) {
      temp = to_string(ob->getPosVal(counter));
      temp.insert(temp.begin(), 4 - temp.size(), ' ');
      boardVals += temp;
   }
   out << "   | " << boardVals << "    |"<<endl;
   temp.clear();
   boardVals.clear();

   temp = to_string(ob->getPosVal(13));
   temp.insert(temp.begin(), 2 - temp.size(), ' ');
   boardVals = "13 |" + temp + "                        ";
   temp = to_string(ob->getPosVal(6));
   temp.insert(temp.begin(), 2 - temp.size(), ' ');
   boardVals += temp + " | 6";

   out << boardVals <<endl;

   temp.clear();
   boardVals.clear();

   for (counter = 0; counter < 6; counter++) {
      temp = to_string(ob->getPosVal(counter));
      temp.insert(temp.begin(), 4 - temp.size(), ' ');
      boardVals += temp;
   }
   out << "   | " << boardVals << "    |"<<endl;
   out << "    -----------------------------" << endl;
   out << "White   0   1   2   3   4   5" <<endl;

   out << (ob->GetWhoseMove() ? "Black" : "White" ) << (ob->GetPassNeeded() ? 
    " must pass": "'s move") <<endl;
}

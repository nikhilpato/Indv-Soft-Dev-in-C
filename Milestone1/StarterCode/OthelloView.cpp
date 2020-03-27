#include "OthelloView.h"
#include "OthelloBoard.h"

using namespace std;

void OthelloView::Draw(ostream &out) {
   string rtn;
   int row, col;
   char sqr;
   shared_ptr<const OthelloBoard> ob
    = dynamic_pointer_cast<const OthelloBoard>(mModel);

   for (row = 0; row < OthelloBoard::dim; row++) {
      for (col = 0; col < OthelloBoard::dim; col++) {
         sqr = ob->GetSquare(row, col);
         rtn = rtn +  sqr == OthelloBoard::mWPiece ? "W"
          : sqr == OthelloBoard::mBPiece ? "B" : ".";
      }
      rtn += "\n";
   }
   rtn += ob->GetWhoseMove() ? "W\n" : "B\n";

   out << rtn;
}

#include <regex>
#include <ctype.h>
#include "MyLib.h"
#include "CheckersMove.h"
#include "CheckersBoard.h"

using namespace std;

vector<CheckersMove *> CheckersMove::mFreeList;

void *CheckersMove::operator new(size_t sz) {
   void *temp;

   if (mFreeList.size()) {
      temp = mFreeList.back();
      mFreeList.pop_back();
   } 
   else
      temp = ::new char[sz];

   mOutstanding++;
   return temp;
}

void CheckersMove::operator delete(void *p) {
   mFreeList.push_back((CheckersMove *)p);
   mOutstanding--;
}

CheckersMove::CheckersMove(const vector<CheckersBoard::Loc> &seq) : 
 mSeq(seq), mWasKinged(false) {}

bool CheckersMove::operator==(const Board::Move &rhs) const {
   const CheckersMove &oRhs = dynamic_cast<const CheckersMove &>(rhs);
   vector<CheckersBoard::Loc>::const_iterator itr1, itr2;
   
   for (itr1 = mSeq.begin(), itr2 = oRhs.mSeq.begin(); itr1 != mSeq.end() &&
    itr2 != oRhs.mSeq.end() && *itr1 == *itr2; itr1++, itr2++)
      ;

   return itr1 == mSeq.end() && itr2 == oRhs.mSeq.end();
}

bool CheckersMove::operator<(const Board::Move &rhs) const {
   const CheckersMove &oRhs = dynamic_cast<const CheckersMove &>(rhs);
   vector<CheckersBoard::Loc>::const_iterator itr1, itr2;
   
   for (itr1 = mSeq.begin(), itr2 = oRhs.mSeq.begin();itr1 != mSeq.end() &&
    itr2 != oRhs.mSeq.end() && *itr1 == *itr2; itr1++, itr2++)
      ;

   if (itr1 == mSeq.end() && itr2 != oRhs.mSeq.end())
      return true;
   else if (itr2 == oRhs.mSeq.end() && itr1 != mSeq.end())
      return false;

   return itr1->row < itr2->row || (itr1->row == itr2->row && itr1->col < 
    itr2->col);
}

CheckersMove::operator string() const {
   string rtn;
   vector<CheckersBoard::Loc>::const_iterator itr;
   
   itr = mSeq.begin();
   if (itr != mSeq.end() && InRange<short>(0, itr->row, CheckersBoard::DIM) && 
    InRange<short>(0, itr->col, CheckersBoard::DIM)) {
      rtn = FString("%c%d", (*itr).row + 'A', (*itr).col + 1);
      for (itr++; itr != mSeq.end(); itr++)
         rtn += FString(" -> %c%d", (*itr).row + 'A', (*itr).col + 1);
   }

   return rtn;
}

void CheckersMove::operator=(const string &src) {
   regex firstPart(R"(^ *([A-Za-z]) *(\d+))");
   regex reMore(R"(^ *-> *([A-Za-z]) *(\d+))");
   smatch reRes;
   
   string line = src;
   const int kRow = 1, kCol = 2;
   int tCol = -1;
   string temp;
   int tRow = -1;
   std::vector<CheckersBoard::Loc> tempSeq;
   bool flagged = false;

   if (regex_search(line, reRes, firstPart)) {
      temp = reRes[kRow];
      tRow = (int)(toupper(temp[0])) - (int)'A';
      tCol = (stoi(reRes[kCol]) - 1);

      if (!InRange<short>(0, tRow, CheckersBoard::DIM) || !InRange<short>(0, 
       tCol, CheckersBoard::DIM))
         flagged = true;

      tempSeq.push_back(CheckersBoard::Loc((short)tRow, (short)tCol));
   } else {
      throw BaseException(FString("Bad Checkers move: %s", src.c_str()));
   }
   
   line = reRes.suffix();
   while (line.size() > 0) {
      if (regex_search(line, reRes, reMore)) {
         temp = reRes[kRow];
         tRow = (int)(toupper(temp[0])) - (int)'A';
         tCol = (stoi(reRes[kCol]) - 1);

         if (!InRange<short>(0, tRow, CheckersBoard::DIM) || !InRange<short>(0, 
          tCol, CheckersBoard::DIM)) {
            flagged = true;
         }
         tempSeq.push_back(CheckersBoard::Loc((short)tRow, (short)tCol));
         line = reRes.suffix();
      } else
         throw BaseException(FString("Bad Checkers move: %s", src.c_str()));
   }  
   
   if (flagged)
      throw BaseException(FString("Out of bounds Checkers move: %s", src.c_str()
       ));
       
   mSeq.clear();
   copy(tempSeq.begin(), tempSeq.end(), back_inserter(mSeq));
}

unique_ptr<Board::Move> CheckersMove::Clone() const {
   return unique_ptr<Move>{new CheckersMove(*this)};
}

istream &CheckersMove::Read(istream &is) {
   char numSteps, row, col;
   mSeq.clear();
   mWereKings.clear();

   is.read(&numSteps, sizeof(numSteps));

   while (numSteps--) {
      is.read((char *)&row, sizeof(row));
      is.read((char *)&col, sizeof(col));
      mSeq.push_back(CheckersBoard::Loc((short)row,(short)col));
   }

   return is;
}

ostream &CheckersMove::Write(ostream &os) const {
   char numSteps, row, col;
   vector<CheckersBoard::Loc>::const_iterator itr;

   numSteps = mSeq.size();
   os.write((char *)&numSteps, sizeof(numSteps));

   for (itr = mSeq.begin(); itr != mSeq.end(); itr++) {
      row = (char)(*itr).row;
      col = (char)(*itr).col;
      os.write((char *)&row, sizeof(row));
      os.write((char *)&col, sizeof(col));
   }

   return os;
}

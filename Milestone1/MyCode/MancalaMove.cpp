#include "MyLib.h"
#include "MancalaMove.h"
#include "MancalaBoard.h"
#include <regex>

using namespace std;

vector<MancalaMove *> MancalaMove::mFreeList;

bool MancalaMove::operator==(const Board::Move &rhs) const {
   const MancalaMove &oRhs = dynamic_cast<const MancalaMove &>(rhs);

   return mPos == oRhs.mPos;
}

bool MancalaMove::operator<(const Board::Move &rhs) const {
   const MancalaMove &oRhs = dynamic_cast<const MancalaMove &>(rhs);

   return mPos < oRhs.mPos;
}

MancalaMove::operator string() const {
   return mPos == -1 ? "Pass" : FString("%d", mPos);
}

void MancalaMove::operator=(const string &src) {
   regex reOth(R"(^ *((Pass)| *(\d+)) *$)");

   const int kPassPtn = 2, kRowPtn = 3; // RE Pattern indexes
   smatch reRes;
   int tPos = -1;  // Assume a "pass"

   if (regex_search(src, reRes, reOth)) {
      if (reRes[kPassPtn].length() == 0) {  // If not pass
         tPos = stoi(reRes[kRowPtn]);

         if (!InRange<short>(0, tPos, MancalaBoard::DIM))
            throw BaseException(FString("Out of bounds Mancala move: %s",
             src.c_str()));
      }
   }
   else 
      throw BaseException(FString("Bad Mancala move: %s", src.c_str()));
   
   mPos = (char)tPos;
}

unique_ptr<Board::Move> MancalaMove::Clone() const {
   return unique_ptr<Move>{new MancalaMove(*this)};
}

void *MancalaMove::operator new(size_t sz) {
   void *temp;

   if (mFreeList.size()) {
      temp = mFreeList.back();
      mFreeList.pop_back();
   } 
   else {
      temp = ::new char[sz];
   }

   mOutstanding++;
   return temp;
}

void MancalaMove::operator delete(void *p) {
   mFreeList.push_back((MancalaMove *)p);
   mOutstanding--;
}

istream &MancalaMove::Read(istream &is) {
   is.read(&mPos, sizeof(mPos));
   is.read((char *)&mWasPassed, sizeof(mWasPassed));
   for (int i = 0; i < MancalaBoard::DIM; i++)
      is.read(&mPrevBoard[i], sizeof(char)); 
   return is;
}

ostream &MancalaMove::Write(ostream &os) const {
   vector<char>::const_iterator itr;
   os.write(&mPos, sizeof(mPos));
   os.write((char *)&mWasPassed, sizeof(mWasPassed));
   for (int i = 0; i < MancalaBoard::DIM; i++)
      os.write(&mPrevBoard[i], sizeof(char));
   return os;
}
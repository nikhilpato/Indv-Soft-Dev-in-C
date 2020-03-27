#include <iostream>
#include <limits.h>
#include <algorithm>
#include <memory>
#include <assert.h>

#include "MancalaDlg.h"
#include "MancalaView.h"
#include "MancalaBoard.h"
#include "MancalaMove.h"
#include "MyLib.h"
#include "BasicKey.h"

using namespace std;

set<MancalaBoard *> MancalaBoard::mRoster;
MancalaBoard::Rules MancalaBoard::mRules;

const BoardClass MancalaBoard::mClass("MancalaBoard", MancalaBoard::CreateBoard,
 "Mancala", &MancalaView::mClass, &MancalaDlg::mClass, MancalaBoard::SetOptions,
  MancalaBoard::GetOptions);

MancalaBoard::MancalaBoard() : mNextMove(mWPiece), mPassNeeded(false) {
   int counter;

   for (counter = 0; counter < DIM; counter++)
      if (counter == DIM-1 || counter == DIM/2-1)
         mBoard[counter] = 0;
      else
         mBoard[counter] = 4;  
   mRoster.insert(this);
}

MancalaBoard::~MancalaBoard() {
   mRoster.erase(this);
}

int MancalaBoard::GetValue() const {
   int sum = mNextMove * mRules.moveWgt;
   int sense;

   for (int i = 0; i < DIM-1; i++) {
      sense = i < DIM/2 ? 1 : -1;
      if (mBoard[i] == 0 && i != DIM/2-1)
         sum += sense * mRules.emptBwlWgt;

      if (i == DIM/2 - 2 && sum == ((mRules.emptBwlWgt * (DIM/2 - 1)) + 
       (mNextMove * mRules.moveWgt)))
         return mBoard[DIM-1] < mBoard[DIM/2 - 1] ? kWinVal : mBoard[DIM-1] 
          > mBoard[DIM/2 - 1] ? -kWinVal : 0;
   }
   sum += (mBoard[DIM/2 -1] -  mBoard[DIM - 1]) * mRules.homeWgt;

   return sum;
}

void MancalaBoard::ApplyMove(unique_ptr<Move> uMove) {
   shared_ptr<Move> ourMove{move(uMove)};      // Take ownership
   shared_ptr<MancalaMove> om = dynamic_pointer_cast<MancalaMove>(ourMove);
   
   int mancalaPos = (mNextMove == mWPiece ? (DIM/2) - 1 : DIM - 1);
   int bottom = (mNextMove == mWPiece ?  0 : (DIM/2));
   int skipPos = (mNextMove == mWPiece ?  DIM - 1: (DIM/2) - 1);

   int counter;
   int posCounter;
   int oppositeOfEmpt;

   char whiteSum = 0;
   char blackSum = 0;

   for (int i = 0; i <DIM; i++)
      om->mPrevBoard[i] = mBoard[i];

   if (om->IsPass()) {
      om->mWasPassed = true;
      mPassNeeded = false;
   } else {
      if (!InRange(bottom, (int)om->mPos, mancalaPos))
         throw BaseException(FString("Invalid move being applied\n"));
      
      posCounter = om->mPos;
      counter = mBoard[posCounter];
      mBoard[posCounter] = 0;

      while (counter--) {
         posCounter = (posCounter + 1) % DIM;
         
         if (posCounter != skipPos)
            mBoard[posCounter]++;
         else
            counter++;
      }

      if (posCounter == mancalaPos) {
         //Ended in your own mancala
         mPassNeeded = true;
      }

      if (mBoard[posCounter] == 1 && bottom <= posCounter && posCounter < 
       mancalaPos) {
         //The bowl was empty before we put our last stone there
         oppositeOfEmpt = DIM - posCounter - 2;
         mBoard[mancalaPos] += mBoard[oppositeOfEmpt] + 1;
         mBoard[oppositeOfEmpt] = 0;
         mBoard[posCounter] = 0;
      }

      //Checks if after the move is made, your side has no more moves
         //Meaning that the game is over
      for (counter = 0; counter < DIM - 1; counter++)
         if (counter < DIM/2 - 1)
            whiteSum += mBoard[counter];
         else if (counter > DIM/2 - 1)
            blackSum += mBoard[counter];

      //If your side doesn't have moves left, you sum the opponents
      //remaining stones and add them to their mancala
      if (whiteSum == 0 || blackSum == 0) {
         mPassNeeded = false;
         for (counter = 0; counter < DIM - 1; counter++)
            if (counter != skipPos && counter != mancalaPos)
               mBoard[counter] = char(0);
         if (whiteSum != 0)
            mBoard[DIM/2 - 1] += whiteSum;
         else
            mBoard[DIM - 1] += blackSum;
      }
   }
   
   mMoveHist.push_back(ourMove);
   mNextMove = -mNextMove; 
}

void MancalaBoard::UndoLastMove() {
   shared_ptr<MancalaMove> om = dynamic_pointer_cast<MancalaMove>(mMoveHist.back
    ());

   if (mMoveHist.size() > 0) {
      mMoveHist.pop_back();
      for (int i = 0; i <DIM; i++)
         mBoard[i] = om->mPrevBoard[i];

      mPassNeeded = om->mWasPassed;
      mNextMove = -mNextMove;
   }
}

void MancalaBoard::GetAllMoves(list<unique_ptr<Move>> *moves) const {
   int mancalaPos;
   int bottom;
   
   if (mPassNeeded) {
      moves->push_back(unique_ptr<Move>{new MancalaMove(-1)});
   } else {
      mancalaPos = (mNextMove == mWPiece ? (DIM / 2) - 1 : DIM - 1);
      bottom = (mNextMove == mWPiece ?  0 : (DIM / 2));

      for (int i = bottom; i < mancalaPos; i++)
         if (mBoard[i] != 0)
            moves->push_back(unique_ptr<Move>{new MancalaMove(i)});
   }
   
}

unique_ptr<Board::Move> MancalaBoard::CreateMove() const {
   return unique_ptr<Move>{new MancalaMove()};
}

unique_ptr<Board> MancalaBoard::Clone() const {
   MancalaBoard *rtn = new MancalaBoard(*this);

   return unique_ptr<Board>(rtn);
}

unique_ptr<const Board::Key> MancalaBoard::GetKey() const {
   BasicKey<KEY_LEN> *rtn = new BasicKey<KEY_LEN>();
   int counter;
   uint *vals = rtn->vals;
   int pos = 0;
   int limit;

   vals[pos] = vals[pos] << oneShift | (mNextMove == mWPiece ? 1 : 0);
   vals[pos] = vals[pos] << oneShift | (mPassNeeded ? 1 : 0);

   for (pos = 0; pos < KEY_LEN; pos++) {
      limit = (pos == KEY_LEN-1 ? 4 : 5);
      for (int i = 0; i < limit; i++)
         vals[pos] = vals[pos] << sixShift | mBoard[i + (pos * 5)];
      if (pos > 0)
         vals[pos] = vals[pos] << twoShift;
   }
   
   vals[KEY_LEN - 1] = vals[KEY_LEN - 1] << sixShift;
 
   return unique_ptr<const Board::Key>(rtn);
}

Object *MancalaBoard::CreateBoard() {return new MancalaBoard();}

void *MancalaBoard::GetOptions() {
   return new Rules(mRules);
}

void MancalaBoard::SetOptions(const void *opts) {
   const Rules *rules = reinterpret_cast<const Rules *>(opts);

   mRules.homeWgt = rules->homeWgt;
   mRules.emptBwlWgt = rules->emptBwlWgt;
   mRules.moveWgt = rules->moveWgt;
}

void MancalaBoard::Rules::EndSwap() {
   homeWgt = EndianXfer(homeWgt);
   emptBwlWgt = EndianXfer(emptBwlWgt);
   moveWgt = EndianXfer(moveWgt);
}

istream &MancalaBoard::Read(istream &is) {
   Rules temp;
   char mvCount;
   char value;
   MancalaMove *myMove;
   mMoveHist.clear();

   is.read((char *)&temp, sizeof(Rules));
   temp.EndSwap();

   SetOptions(&temp);

   is.read(&mNextMove, sizeof(mNextMove));
   is.read((char *) &mPassNeeded, sizeof(mPassNeeded));

   for (int i = 0; i < DIM; i++)
      is.read(&mBoard[i], sizeof(char));

   is.read((char *)&mvCount, sizeof(mvCount));

   while (is && mvCount--) {
      myMove = new MancalaMove();
      is >> *myMove;
      mMoveHist.push_back(shared_ptr<MancalaMove>(myMove));
   }

   return is;
}

ostream &MancalaBoard::Write(ostream &os) const {
   Rules rls = mRules;
   char mvCount = mMoveHist.size();
   
   rls.EndSwap();
   os.write((char *)&rls, sizeof(rls));

   os.write(&mNextMove, sizeof(mNextMove));
   os.write((char *) &mPassNeeded, sizeof(mPassNeeded));

   for (int i = 0; i < DIM; i++)
      os.write(&mBoard[i], sizeof(char));

   os.write((char *)&mvCount, sizeof(mvCount));
   for (auto &itr : mMoveHist)
      os << *itr;

   return os;
}




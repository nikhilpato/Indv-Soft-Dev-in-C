#include <iostream>
#include <limits.h>
#include <algorithm>
#include <memory>
#include <assert.h>

#include "CheckersDlg.h"
#include "CheckersView.h"
#include "CheckersBoard.h"
#include "CheckersMove.h"
#include "MyLib.h"
#include "BasicKey.h"

using namespace std;

CheckersBoard::Rules CheckersBoard::mRules;

const BoardClass CheckersBoard::mClass("CheckersBoard", 
 CheckersBoard::CreateBoard, "Checkers", &CheckersView::mClass,   
  &CheckersDlg::mClass, CheckersBoard::SetOptions, CheckersBoard::GetOptions);

CheckersBoard::Direction CheckersBoard::mDirs[] = {
 {1, -1}, {1, 1}, {-1, 1}, {-1, -1}
};

set<CheckersBoard *> CheckersBoard::mRoster;

CheckersBoard::CheckersBoard(void) : mMoveFlg(0), mMovesValid(false) {
   int row, col;
   
   for (row = 0; row < DIM; row++)
      for (col = 0; col < DIM; col++)
         if (row != DIM/2-1 && row != DIM/2 && (row + col) % 2 != 1)
            mBoard[row][col] = PIECE | (row > DIM/2 ? WHITE : 0);
         else
            mBoard[row][col] = NONE;
   mValue += mRules.moveWgt;
   mRoster.insert(this);
}

CheckersBoard::~CheckersBoard() {
   mRoster.erase(this);
}

int CheckersBoard::GetValue() const {
   if (!mMovesValid)
      CalcMoves();
      
   return mMoves.size() > 0 ? mValue : (mMoveFlg == WHITE) ? kWinVal : -kWinVal;
}

void CheckersBoard::ApplyMove(unique_ptr<Move> uMove) {
   shared_ptr<Move> ourMove{move(uMove)};
   shared_ptr<CheckersMove> mv = dynamic_pointer_cast<CheckersMove>(ourMove);
   Loc here, there, original;
   char piece, jmpPiece;
   int step, sense = (mMoveFlg == WHITE ? -1 : 1);

   if (mv->mSeq.size() == 0) {
      throw BaseException(FString("Invalid move being applied\n"));
   }
   
   if (mv->mSeq.size() > 1) {
      auto itr = mv->mSeq.begin();
      original = here = *itr;
      if (!InRange(here))
         throw BaseException(FString("Invalid move being applied\n"));

      piece = mBoard[here.row][here.col];
      if (piece == NONE || ((piece & WHITE) != mMoveFlg))
         throw BaseException(FString("Invalid move being applied\n"));
      
      mv->mWasKinged = false;

      if (IsBackRow(piece, here))
         mValue -= sense * mRules.backWgt;

      step = mv->mSeq[0].row - mv->mSeq[1].row;
      if (step > 1 || step < -1) // Do jumps
         for (itr++; itr != mv->mSeq.end(); itr++) {
            there = *itr;
            if (!InRange(there))
               throw BaseException(FString("Invalid move being applied\n"));
            jmpPiece = mBoard[(here.row+there.row)/2][(here.col+there.col)/2];
            mBoard[(here.row + there.row)/2][(here.col + there.col)/2] = 0;
            assert(jmpPiece != NONE && (jmpPiece & WHITE) != (piece & WHITE));
            mv->mWereKings.push_back((jmpPiece & KING));
            mValue += sense * (jmpPiece & KING ? mRules.kingWgt : PIECEWGT);
            
            here = there;
         }
      else // Do simple move
         here = mv->mSeq.back();

      // King me
      if (((piece & WHITE) && here.row == 0 || !(piece & WHITE) && here.row == 
       DIM-1) && !(piece & KING)) {
         mValue += sense * (mRules.kingWgt - PIECEWGT);
         piece |= KING;
         mv->mWasKinged = true;
      }
      //If it ends up in the last row
      if (IsBackRow(piece, here))
         mValue += sense * mRules.backWgt;

      mBoard[here.row][here.col] = piece;
      mBoard[original.row][original.col] = 0;

      mMoveFlg ^= WHITE;
      mValue -= 2*sense*mRules.moveWgt;
      mMovesValid = false;
      mMoveHist.push_back(ourMove);
   }
}

void CheckersBoard::UndoLastMove() {
   shared_ptr<CheckersMove> mv = 
    dynamic_pointer_cast<CheckersMove>(mMoveHist.back());
   Loc here, there, original;
   char piece;
   int step, sense = (mMoveFlg == WHITE ? -1 : 1);
   //Move flag is currently in the opponents state
   piece = (mMoveFlg == WHITE ? WHITE : PIECE);

   if (mMoveHist.size() > 0) {
      mMoveHist.pop_back();
      auto itr = mv->mSeq.begin();
      auto itr2 = mv->mWereKings.begin();
      original = here = *itr;

      step = mv->mSeq[0].row - mv->mSeq[1].row;
      if (step > 1 || step < -1) {
         for (itr++; itr != mv->mSeq.end(); itr++, itr2++) {
            there = *itr;
            
            if ((*itr2 & KING)) {
               mBoard[(here.row + there.row)/2][(here.col + there.col)/2] = 
                (piece | KING);
               mValue += sense * mRules.kingWgt;
            } else {
               mBoard[(here.row + there.row)/2][(here.col + there.col)/2] = 
                piece;
               mValue += sense * PIECEWGT;
            }
            here = there;
         }
      }
      
      if (mv->mWasKinged) {
         mValue += sense * (mRules.kingWgt - PIECEWGT);
         piece = (mMoveFlg == WHITE ? PIECE : WHITE);
      } else {
         piece = mBoard[mv->mSeq.back().row][mv->mSeq.back().col];
      }
      
      mBoard[original.row][original.col] = piece;
      mBoard[mv->mSeq.back().row][mv->mSeq.back().col] = 0;

      if (IsBackRow(piece, original))
         mValue -= sense * mRules.backWgt;

      if (IsBackRow(piece, mv->mSeq.back()))
         mValue += sense * mRules.backWgt;

      mValue -= 2*sense*mRules.moveWgt;
   
      mMoveFlg ^= WHITE;
      mMovesValid = false;
   }
}

void CheckersBoard::GetAllMoves(list<unique_ptr<Move>> *moves) const {
   if (!mMovesValid)
      CalcMoves();
   for (auto &uptr : mMoves)
      moves->push_back(unique_ptr<Move>{ new CheckersMove(uptr) });
}

unique_ptr<Board::Move> CheckersBoard::CreateMove() const {
   return unique_ptr<Move>(new CheckersMove(vector<Loc>()));
}

unique_ptr<Board> CheckersBoard::Clone() const {
   CheckersBoard *rtn = new CheckersBoard(*this);
   return unique_ptr<Board>(rtn);
}

unique_ptr<const Board::Key> CheckersBoard::GetKey() const {
   BasicKey<KEY_LEN> *rtn = new BasicKey<KEY_LEN>();
   int row, col;
   uint *vals = rtn->vals;
   int pos = 0;
   int moveCounter = -1;

   //Changed: this one used to be |= instead of just =
   vals[pos] = vals[pos] << WHITE | mMoveFlg == WHITE ? WHITE : PIECE;
   for (row = 0; row < DIM; row++)
      for (col = 0; col < DIM; col++) {
         if ((row + col) % 2 != 1) {
            if (moveCounter < 7) {
               moveCounter++;
            } else {
               if (pos == 0)
                  vals[pos] = vals[pos] << sixShift;
               else
                  vals[pos] = vals[pos] << charShift;
               pos++;
               moveCounter = 0;
            }
            vals[pos] = vals[pos] << sqrShift | mBoard[row][col];
         }
      }
   vals[pos] = vals[pos] << charShift;
   return unique_ptr<const Key>(rtn);
}

Object *CheckersBoard::CreateBoard() {return new CheckersBoard();}

void *CheckersBoard::GetOptions() {
   return new Rules(mRules);
}

void CheckersBoard::SetOptions(const void *opts) {
   const Rules *rules = reinterpret_cast<const Rules *>(opts);
   set<CheckersBoard *>::iterator itr;

   mRules.kingWgt = rules->kingWgt;
   mRules.backWgt = rules->backWgt;
   mRules.moveWgt = rules->moveWgt;

   for (itr = mRoster.begin(); itr != mRoster.end(); itr++)
      (*itr)->NewOptions();
}

void CheckersBoard::Rules::EndSwap() {
   kingWgt = EndianXfer(kingWgt);
   backWgt = EndianXfer(backWgt);
   moveWgt = EndianXfer(moveWgt);
}


istream &CheckersBoard::Read(istream &is) {
   int mvCount = 0;
   unique_ptr<CheckersMove> myMove(new CheckersMove(vector<Loc>()));
   Rules temp;

   int clearSize = mMoveHist.size();
   mValue = 0;
   
   for (int i = 0; i <= clearSize; i++)
      UndoLastMove();

   //Read the rules
   is.read((char *)&temp, sizeof(Rules));
   temp.EndSwap();

   SetOptions(&temp);

   is.read((char *)&mvCount, sizeof(mvCount));
   mvCount = EndianXfer(mvCount);
   while (is && mvCount--) {
      is >> *myMove;
      ApplyMove(myMove->Clone());
   }

   //Recalculate the weights
   NewOptions();

   //Set moves valid to false so it forces calc moves
   //to run again
   mMovesValid = false;
   myMove.reset();
   
   return is;
}

ostream &CheckersBoard::Write(ostream &os) const {
   Rules rls = mRules;
   int mvCount = EndianXfer((int)mMoveHist.size());
   
   rls.EndSwap();
   os.write((char *)&rls, sizeof(rls));

   os.write((char *)&mvCount, sizeof(mvCount));
   for (auto &itr : mMoveHist)
      os << *itr;

   return os;
}

void CheckersBoard::NewOptions() {
   list<Move *>::iterator itr;
   char piece;

   mValue = mMoveFlg == WHITE ? -mRules.moveWgt : mRules.moveWgt;
   
   for (int row = 0; row < DIM; row++)
      for (int col = 0; col < DIM; col++)
         if (piece = mBoard[row][col]) {
            mValue += (piece & WHITE ? -1 : 1) * 
             (piece & KING ? mRules.kingWgt : PIECEWGT);
            if (row == 0 && !(piece & WHITE) || row == DIM-1 && (piece & WHITE))
               mValue += (piece & WHITE ? -1 : 1) * mRules.backWgt;
         }
}

// Precompute all possible moves.  Do not alter any existing code here!!  All
// provided code is correct.  Fill in the move-forward section only.

void CheckersBoard::CalcMoves() const {
   int row, col, forward = mMoveFlg == WHITE ? -1 : 1, numDirs;
   uint ndx;
   char piece, jumpPiece;
   Loc thisLoc, jumpLoc, toLoc;
   vector<CheckersBoard::Loc>::const_iterator itr1, itr2;
   bool flag;
   vector<Loc> locs;  // Series of locations moved to
   vector<int> dirs;  // Series of directions moved
   bool upStep;       // Did we just make a new step forward?
   
   mMoves.clear();
   for (row = 0; row < DIM; row++) {
      for (col = 0; col < DIM; col++) {
         piece = mBoard[row][col];

         if (piece && (piece & WHITE) == mMoveFlg) {

            locs.clear();
            locs.push_back(Loc(row, col));
            numDirs = piece & KING ? KING_MOVES : PC_MOVES;
            dirs.push_back(0);
            upStep = false;

            // Run a DFS exploring all possible jump moves.
            while (!locs.empty()) {
               if (dirs.back() == numDirs) {  // If we've explored all dirs
                  if (upStep)
                     mMoves.insert(locs);

                  locs.pop_back();           // Pop back to earlier position
                  dirs.pop_back();
                  
                  if (!dirs.empty())         // Advance backtracked dir by one
                     dirs.back()++;
                  upStep = false; 
               } 
               else {
                  thisLoc = locs.back();

                  toLoc = Loc(
                   thisLoc.row + 2*forward*mDirs[dirs.back()].rDelta,
                   thisLoc.col + 2*mDirs[dirs.back()].cDelta
                );

                  if (InRange(toLoc) && (!mBoard[toLoc.row][toLoc.col] ||
                   toLoc.row == row && toLoc.col == col)) {  
                      // Fill in forward-step logic in DFS here.  Should be
                      // 1-2 dozen lines of code.
                        jumpPiece = mBoard[(thisLoc.row + toLoc.row)/2][
                         (thisLoc.col + toLoc.col)/2];
                        flag = true;
                     for (itr1 = locs.begin(); itr1 != locs.end(); itr1++) {
                        if (toLoc == *itr1) {
                           flag = false;
                           break;
                        }
                     }

                      if (flag && jumpPiece &&
                       ((jumpPiece & WHITE) != mMoveFlg)) {
                         locs.push_back(toLoc);
                         dirs.push_back(-1);
                         upStep = true;
                      }
                      
                     }
                  dirs.back()++;
               }
            }
         }
      }
   }
   if (mMoves.size() == 0) { // If no capture moves
      FindNormalMoves();
   }      
   mMovesValid = true;
}

// Find non-jumping moves
void CheckersBoard::FindNormalMoves() const {
   vector<Loc> locs = vector<Loc>(2);
   int piece, row, col, ndx, forward = mMoveFlg == WHITE ? -1 : 1, numDirs;
   Loc toLoc;

   for (row = 0; row < DIM; row++)
      for (col = 0; col < DIM; col++) {
         piece = mBoard[row][col];

         if (piece && (piece & WHITE) == mMoveFlg) {
            numDirs = piece & KING ? KING_MOVES : PC_MOVES;
            locs[0] = Loc(row, col);
            for (ndx = 0; ndx < numDirs; ndx++) {
               toLoc = Loc(row + forward*mDirs[ndx].rDelta, col + mDirs[ndx].
                cDelta);
               if (InRange(toLoc) && mBoard[toLoc.row][toLoc.col] == 0) {
                  locs[1] = toLoc;
                  mMoves.insert(CheckersMove(locs));
               }
            }
         }
      }
   }        

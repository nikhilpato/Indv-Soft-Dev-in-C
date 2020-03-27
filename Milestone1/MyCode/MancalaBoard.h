#ifndef MANCALABOARD_H
#define MANCALABOARD_H

#include <iostream>
#include <set>
#include "MyLib.h"
#include "Board.h"


class MancalaBoard : public Board {
public:
   friend class MancalaMove;
   static constexpr int KEY_LEN = 3, DIM = 14, oneShift = 0x1, sixShift = 0x6;
   enum {mWPiece = 1, mBPiece = -1};


   struct Rules {
      int homeWgt;
      int emptBwlWgt;
      int moveWgt;

      Rules() : homeWgt(400), emptBwlWgt(200), moveWgt(200) {}

      void EndSwap();
   };

   MancalaBoard();
   ~MancalaBoard();

   const Class *GetClass() const { return &mClass;};

   int GetValue() const override;
   void ApplyMove(std::unique_ptr<Move>) override;
   void UndoLastMove() override;

   void GetAllMoves(std::list<std::unique_ptr<Move>> *) const override;
   std::unique_ptr<Move> CreateMove() const override;

   int GetWhoseMove() const override {return (int)mNextMove == mWPiece;}
   bool GetPassNeeded() const {return mPassNeeded;}
   const std::list<std::shared_ptr<const Move>> &GetMoveHist() const override
    {return *(std::list<std::shared_ptr<const Move>> *)&mMoveHist;}

   std::unique_ptr<Board> Clone() const override;
   std::unique_ptr<const Key> GetKey() const override;

   static Object *CreateBoard();

   static void *GetOptions();
   static void SetOptions(const void *opts); 

   char getPosVal(int pos) const {return mBoard[pos];}

protected:
   std::istream &Read(std::istream &);
   std::ostream &Write(std::ostream &) const;
   
   static std::set<MancalaBoard *> mRoster;
   static Rules mRules;
   const static BoardClass mClass;

   char mBoard[DIM];
   char mNextMove;
   mutable bool mPassNeeded;   
   
   std::list<std::shared_ptr<Move>> mMoveHist;
};




#endif
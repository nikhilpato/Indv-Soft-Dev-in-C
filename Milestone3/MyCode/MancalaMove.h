#ifndef MANCALAMOVE_H
#define MANCALAMOVE_H

#include <iostream>
#include <list>
#include <vector>
#include <memory>
#include "MancalaBoard.h"

class MancalaMove : public Board::Move {
public:
   friend class MancalaBoard;
   MancalaMove(char pos = 0) : mPos(pos), mWasPassed(false) {}

   virtual ~MancalaMove() {}

   bool operator==(const Board::Move &rhs) const; 
   bool operator<(const Board::Move &rhs) const;
   operator std::string() const;
   void operator=(const std::string &src);
   std::unique_ptr<Board::Move> Clone() const;

   bool IsPass() const {return mPos == -1;}
   short GetPos() const {return mPos;}

   void operator delete(void *p);
   void *operator new(size_t sz);

protected:
   std::istream &Read(std::istream &is);
   std::ostream &Write(std::ostream &) const;

   char mPos;
   char mPrevBoard[MancalaBoard::DIM];
   bool mWasPassed;

   static std::vector<MancalaMove *> mFreeList;
};

#endif
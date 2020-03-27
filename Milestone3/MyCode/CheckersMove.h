#ifndef CHECKERSMOVE_H
#define CHECKERSMOVE_H

#include <string>
#include <vector>
#include <memory>
#include "CheckersBoard.h"

class CheckersMove : public Board::Move {
public:
   friend class CheckersBoard;

   CheckersMove(const std::vector<CheckersBoard::Loc> &seq);
   virtual ~CheckersMove() {}

   bool operator==(const Board::Move &rhs) const; 
   bool operator<(const Board::Move &rhs) const;
   operator std::string() const;
   void operator=(const std::string &src);
   std::unique_ptr<Board::Move> Clone() const;

   void operator delete(void *p);
   void *operator new(size_t sz);
   void *operator new(size_t sz, void *vp) {return vp;}

protected:
   std::istream &Read(std::istream &is);
   std::ostream &Write(std::ostream &) const;

   std::vector<CheckersBoard::Loc> mSeq;
   std::vector<char> mWereKings;
   bool mWasKinged;


   static std::vector<CheckersMove *> mFreeList;
};

#endif

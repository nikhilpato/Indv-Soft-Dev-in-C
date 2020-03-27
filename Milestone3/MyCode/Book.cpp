#include "Book.h"
#include "Board.h"
#include <iostream>
#include <stdlib.h>
#include "Class.h"
#include "BasicKey.h"

using namespace std;

istream &Book::Read(std::istream &is, const Class *brdCls){
   const BoardClass *myBoardClass;
   myBoardClass = dynamic_cast<const BoardClass*>(brdCls);
   Board *tempBoard = dynamic_cast<Board *>(brdCls->NewInstance());
   shared_ptr<Board> mB{move(tempBoard)};
   shared_ptr<Board> myBoard = dynamic_pointer_cast<Board>(mB);

   shared_ptr<Board::Move> oMove = myBoard->CreateMove();
   shared_ptr<Board::Move> rMove = myBoard->CreateMove();
   Board::Key *inputKey;
   
   long val = 0;
   char level;
   char replyFlag;

   is.read(&level, sizeof(char));
   SetLevel((int)level);

   while(is.peek() != EOF){
      inputKey = dynamic_cast<Board::Key *>(brdCls->NewInstance());

      is >> *inputKey;
      is >> *oMove;

      is.read(&replyFlag, sizeof(replyFlag));
      if(replyFlag)
         is >> *rMove;

      is.read((char *)&val, sizeof(val));
      val = EndianXfer(val);
      
      unique_ptr<const Board::Key> myKey(move(inputKey));
      (*this)[move(myKey)] =  BestMove(oMove, replyFlag ? rMove : 0, val, 
       mLevel, 0);
   }
   return is;
}

ostream &Book::Write(std::ostream &os){
   Book::iterator itr;
   long val;
   char level = mLevel;
   char replyFlag;

   os.write(&level, sizeof(level));

   for (itr = begin(); itr != end(); itr++){
      os << *(itr->first);
      os << *(itr->second.bestMove);
      if(itr->second.reply){
         replyFlag = 1;
         os.write(&replyFlag, sizeof(replyFlag));
         os << *(itr->second.reply);
      } else {
         replyFlag = 0;
         os.write(&replyFlag, sizeof(replyFlag));
      }

      val = EndianXfer(itr->second.value);
      os.write((char *)&val, sizeof(val));
   }

   return os;
}
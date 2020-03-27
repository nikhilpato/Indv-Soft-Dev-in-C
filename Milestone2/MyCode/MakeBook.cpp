#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Book.h"
#include "SimpleAIPlayer.h"
#include "BestMove.h"
#include "Board.h"
#include "Class.h"
#include "View.h"

using namespace std;

void MakeNewBook(Book *myBook, shared_ptr<Board> myBoard, View *myView, int 
 level, int depth, int dbg, bool ttNeeded) {
   list<unique_ptr<Board::Move>> moves;
   list<unique_ptr<Board::Move>>::iterator mItr;
   BestMove bMove;
   unique_ptr<const Board::Key> key;
   SimpleAIPlayer::TranspositionTable *tt = 0;

   myView->Draw(cout);

   if (myBook->find(key = myBoard->GetKey()) == myBook->end()) {
      if (ttNeeded)
         tt = new SimpleAIPlayer::TranspositionTable();

      SimpleAIPlayer::Minimax(myBoard.get(), level, -Board::kWinVal - 1, 
       Board::kWinVal + 1, &bMove, tt, dbg);

      if (ttNeeded)
         delete tt;

      cout << "Best move: " << (string)(*bMove.bestMove) << " with reply ";
      if (bMove.reply)
         cout << (string)(*bMove.reply) <<endl;
      else
         cout << "unknown" <<endl;
      
      cout << " Boards examined: " << bMove.numBoards << " Value: " << bMove.
       value << endl;

      (*myBook)[move(key)] = bMove;

      cout << "Added as board " << myBook->size() <<endl<<endl;

      if (depth > 0) {
         myBoard->GetAllMoves(&moves);

         for (mItr = moves.begin(); mItr != moves.end(); mItr++) {
            myBoard->ApplyMove(move(*mItr));
            MakeNewBook(myBook, myBoard, myView, level, depth - 1, dbg, 
             ttNeeded);
            myBoard->UndoLastMove();
         }
      }
   } else
      cout << "Duplicate. No book entry." <<endl<<endl;
}

int main() {
   string line, filename;
   vector <string> args;
   string token;
   int level, depth;
   const Class *myViewClass;
   const BoardClass *myBoardClass;
   Board *tempBoard = 0;
   View *myView = 0;
   Book *myBook = new Book();
   ofstream outputFile;
   std::stringstream ss;

   cout << "Enter boardType, level, depth, and filename: ";
   getline(cin, line);
   ss << line;
   args.clear();
   while (ss >> token) {
      args.push_back(token);
   }

   try {
      myBoardClass = dynamic_cast<const BoardClass*>(Class::ForName(args.at(0)))
       ;
      level = stoi(args.at(1));
      depth = stoi(args.at(2));
      filename = args.at(3);
   } catch (out_of_range const &e) {
      return 0;
   }

   myViewClass = myBoardClass->GetViewClass();
   myView = dynamic_cast<View *>(myViewClass->NewInstance());
   tempBoard = dynamic_cast<Board *>(myBoardClass->NewInstance());
   shared_ptr<Board> mB{move(tempBoard)};
   shared_ptr<Board> myBoard = dynamic_pointer_cast<Board>(mB);
   myView->SetModel(myBoard);
      
   MakeNewBook(myBook, myBoard, myView, level, depth, 0, 
    myBoardClass->UseTransposition());

   outputFile.open(filename);
   myBook->Write(outputFile);
   outputFile.close();

   cout << "Writing book... done" <<endl;
   cout << "Before clearing book, moves/keys: " << Board::Move::GetOutstanding
    () << '/' << Board::Key::GetOutstanding() <<endl;
   delete myBook;
   cout << "Final count, moves/keys: " << Board::Move::GetOutstanding() << '/' 
    << Board::Key::GetOutstanding() <<endl;
}

// Author: Nikhil Patolia
// Class: CSC 305-03
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <list>
#include <sstream>
#include "MyLib.h"
#include "Board.h"
#include "View.h"
#include "Dialog.h"
#include "Class.h"

using namespace std;

//Prints the moves, evenly spaced to the size of the longest move and wraps 
 //each line at 80 characters
void PrintMoves(const list<shared_ptr<const Board::Move>> &lst) {
   string s;
   string temp;
   int lineLength = 0;
   int maxSize = 0;

   for (auto const eachMove : lst)
      if (((string)*eachMove).size() > maxSize)
         maxSize = ((string)*eachMove).size();

   maxSize++;
   for (auto const eachMove : lst) {
      temp = (string)*eachMove;
      temp.resize(maxSize, ' ');

      if (lineLength + maxSize > 80) {
         s.pop_back();
         s += '\n';
         lineLength = 0;
      }
      s += temp;

      lineLength += maxSize;
   }

   if (s.size() > 0) {
      s.pop_back();
   }
   if (s.size() > 0)
      cout << s <<endl;
}

//Runs the program, first by getting the type of board through the command line 
 //and then accepting commands until the program is quit
int main(int argc, char ** argv) {
   const Class *myViewClass, *myDlgClass;
   const BoardClass *myBoardClass;
   Board *tempBoard = 0;
   View *myView = 0;
   Dialog *myDlg = 0;
   unique_ptr<Board::Move> oMove, compareMove;
   string mFriendlyName;

   if (argc < 2 || argc > 3) {
      cout << "Usage: BoardTest BoardClass [runLimit]\n" <<
       "Failed to create classes or objects" <<endl;
      return 0;
   } else if (!(myBoardClass = dynamic_cast<const BoardClass*>
    (Class::ForName(argv[1])))) {
      cout << "Unknown board type: " << argv[1] 
       << "\nFailed to create classes or objects" <<endl;
      return 0;
   }

   myViewClass = myBoardClass->GetViewClass();
   myDlgClass = myBoardClass->GetDlgClass();
   myView = dynamic_cast<View *>(myViewClass->NewInstance());
   myDlg = dynamic_cast<Dialog *>(myDlgClass->NewInstance());
   mFriendlyName = myBoardClass->GetFriendlyName();   

   tempBoard = dynamic_cast<Board *>(myBoardClass->NewInstance());
   shared_ptr<Board> mB{move(tempBoard)};

   shared_ptr<Board> myBoard = dynamic_pointer_cast<Board>(mB);
   oMove = myBoard->CreateMove();
   myView->SetModel(myBoard);

   void *rules;
   list<unique_ptr<Board::Move>> lst;
   list<shared_ptr<const Board::Move>> cLst;

   ofstream outputFile;
   vector <string> args;
   string token;
   string line;

   bool flag;
   int seedNum;
   int numMoves;
   int numRetract;
   int inputMoves;

   while (getline(cin, line)) {
      if (cin.eof()) {
         cout << "Error: Unexpected EOF\n"<<endl;
         return 0;
      }

      std::stringstream ss(line);
      args.clear();
      while (ss >> token) {
         args.push_back(token);
      }

      try {
         if (args.at(0) == "enterMove") {
            try {
               *oMove = line.substr(9);
            } catch (BaseException const &e) {
               cout << "Error: " <<  e.what() <<endl;
            }

         } else if (args.at(0) == "showMove") {
            cout << (string) *oMove <<endl;

         } else if (args.at(0) == "applyMove") {
            flag = true;
            lst.clear();
            myBoard->GetAllMoves(&lst);
            for (auto &uptr : lst) {
               if (*uptr == *oMove)
                  try {
                     myBoard->ApplyMove(oMove->Clone());
                     flag = false;
                     break;
                  } catch (BaseException const &e) {
                     cout << e.what() <<endl;
                  }                 
            }
            if (flag)
               cout << "Invalid move being applied" <<endl;

         } else if (args.at(0) == "doMove") {
            try {
               (*oMove) = line.substr(6);
               flag = true;
               lst.clear();
               myBoard->GetAllMoves(&lst);
               for (auto &uptr : lst) {
                  if (*uptr == *oMove)
                     try {
                        myBoard->ApplyMove(oMove->Clone());
                        flag = false;
                        break;
                     } catch (BaseException const &e) {
                        cout << e.what() <<endl;
                     }                 
               }
               if (flag)
                  cout << "Invalid move being applied" <<endl;
            } catch (BaseException const &e) {
               cout << "Error: " << e.what() <<endl;
            }

         } else if (args.at(0) == "compareMove") {
            compareMove = myBoard->CreateMove();
            try {
               *compareMove = line.substr(11);
               if (*oMove == *compareMove)
                  cout << "Moves are equal" <<endl;
               else if (*oMove < *compareMove)
                  cout << "Current move is less than entered move" <<endl;
               else if (*compareMove < *oMove)
                  cout << "Current move is greater than entered move" <<endl;
               compareMove.reset();
            } catch (BaseException const &e) {
               cout << "Error: " <<  e.what() <<endl;
            }
            
         } else if (args.at(0) == "undoLastMove") {
            try {
               inputMoves = stoi(args.at(1));
               numMoves = min(int(myBoard->GetMoveHist().size()), inputMoves);
               for (int i = 0; i < numMoves; i++)
                  myBoard->UndoLastMove();
            }
            catch (invalid_argument const &e) {
               cout << "Error: Must have a nonnegative count for undoLastMove"  
                <<endl;
            } catch (out_of_range const &e) {
               cout << "Error: Must have a nonnegative count for undoLastMove" 
                <<endl;
            }

         } else if (args.at(0) == "showVal") {
            cout << "Value: " << myBoard->GetValue() <<endl;

         } else if (args.at(0) == "showMoveHist") {
            cLst.clear();
            cLst = myBoard->GetMoveHist();
            cout << "\nMove History: " <<endl;
            PrintMoves(cLst);

         } else if (args.at(0) == "showBoard") {
            myView->Draw(cout);
            lst.clear();
            myBoard->GetAllMoves(&lst);
            cLst.clear();
            for (auto &uptr : lst) {
               cLst.push_back(std::move(uptr));                        
            } 
            cout << "\nAll Moves: " <<endl;
            PrintMoves(cLst);

         } else if (args.at(0) == "testPlay") {
            try {
               seedNum = stoi(args.at(1));
               numMoves = stoi(args.at(2));
               srand (seedNum);
               for (int i = 0; i < numMoves; i++) {
                  lst.clear();
                  myBoard->GetAllMoves(&lst);
                  if (lst.size() == 0) {
                     break;
                  }
                  auto itr = lst.begin();
                  advance(itr, rand() % lst.size());
                  myBoard->ApplyMove(move(*itr));
               }
            } catch (invalid_argument const &e) {
               cout << "Error: Bad arguments for testPlay/testRun" <<endl;
            } catch (out_of_range const &e) {
               cout << "Error: Bad arguments for testPlay/testRun" <<endl;
            }

         } else if (args.at(0) == "testRun") {
            try {
               seedNum = stoi(args.at(1));
               numMoves = stoi(args.at(2));
               srand (seedNum);

               for (int i = 0; i < numMoves; i++) {
                  lst.clear();
                  myBoard->GetAllMoves(&lst);
                  if (lst.size() == 0) {
                     numRetract = (rand() % myBoard->GetMoveHist().size()) + 1;
                     for (int x = 0; x < numRetract; x++)
                        myBoard->UndoLastMove();
                  } else {
                     auto itr = lst.begin();
                     advance(itr, rand() % lst.size());
                     myBoard->ApplyMove(move(*itr));
                  }
               }
            } catch (invalid_argument const &e) {
               cout << "Error: Bad arguments for testPlay/testRun" <<endl;
            } catch (out_of_range const &e) {
               cout << "Error: Bad arguments for testPlay/testRun" <<endl;
            }

         } else if (args.at(0) == "saveMove") {
            try {
               ofstream outputFile;
               outputFile.open(args.at(1));
               outputFile << *oMove;
               outputFile.close();
            } catch (invalid_argument const &e) {
               cout << "Error: Bad saveMove argument" <<endl;
            } catch (out_of_range const &e) {
               cout << "Error: Bad saveMove argument" <<endl;
            }

         } else if (args.at(0) == "loadMove") {
         try {
            ifstream inputFile;
            inputFile.open(args.at(1));
            if (inputFile.good())
               inputFile >> *oMove;
            else
               cout << "Error: Bad file in loadMove" <<endl;

            inputFile.close();
         } catch (invalid_argument const &e) {
            cout << "Error: Bad loadMove argument" <<endl;
         } catch (out_of_range const &e) {
            cout << "Error: Bad loadMove argument" <<endl;
         }

         } else if (args.at(0) == "saveBoard") {
            try {
               ofstream outputFile;
               outputFile.open(args.at(1));
               outputFile << *myBoard;
               outputFile.close();
            } catch (invalid_argument const &e) {
               cout << "Error: Bad saveBoard argument" <<endl;
            } catch (out_of_range const &e) {
               cout << "Error: Bad saveBoard argument" <<endl;
            }

         } else if (args.at(0) == "loadBoard") {
            try {
               ifstream inputFile;
               inputFile.open(args.at(1));
               if (inputFile.good())
                  inputFile >> *myBoard;
               else
                  cout << "Error: Bad file in loadBoard" <<endl;

               inputFile.close();
            } catch (invalid_argument const &e) {
               cout << "Error: Bad loadBoard argument" <<endl;
            } catch (out_of_range const &e) {
               cout << "Error: Bad loadBoard argument" <<endl;
            }   
         
         } else if (args.at(0) == "setOptions") {
            rules = myBoardClass->GetOptions();
            if (myDlg->Run(cin, cout, rules))
               myBoardClass->SetOptions(rules);

            delete rules;

         } else if (args.at(0) == "compareKeys") {
            try {
               ifstream inputFile;
               inputFile.open(args.at(1));
               unique_ptr<Board> compareBoard = myBoard->Clone();
               if (inputFile.good()) {
                  inputFile >> *compareBoard;
                  if (*(myBoard->GetKey()) == *(compareBoard->GetKey()))
                     cout << "Board keys are equal" <<endl;
                  else {
                     cout << "Board keys are unequal" <<endl;
                     if (*(myBoard->GetKey()) < *(compareBoard->GetKey()))
                        cout << "Current board is less than " << args.at(1) 
                         <<endl;
                     else
                        cout << "Current board is greater than " << args.at(1) 
                         <<endl;
                  } 
               } else
                  cout << "Error: Bad file in compareKeys" <<endl;;

               inputFile.close();
               compareBoard.reset(); 
            } catch (invalid_argument const &e) {
               cout << "Error: Bad loadBoard argument" <<endl;
            } catch (out_of_range const &e) {
               cout << "Error: Bad loadBoard argument" <<endl;
            }
         } else if (args.at(0) == "keyMoveCount") {
            lst.clear();
            cLst.clear();
            cout << "Moves/Keys: " << Board::Move::GetOutstanding() << '/' << 
             Board::Key::GetOutstanding() <<endl; 
         } else if (args.at(0) == "quit")
            return 0;
         else 
            cout << "Unknown command: " << line << endl;
      } catch (out_of_range const &e) {
      } catch (BaseException &e) {
         cout << "Error: " << e.what() <<endl;
      }
      if (args.size() > 0)
         cout <<endl;
   }
}

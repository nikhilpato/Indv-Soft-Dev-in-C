#include <sstream>
#include <fstream>
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "Board.h"
#include "MyLib.h"

using namespace std;

HumanPlayer::NameMap HumanPlayer::mNameMap;
int HumanPlayer::mLevel;

HumanPlayer::HumanPlayer(shared_ptr<GameModel> mdl, std::string name) : 
 mModel(mdl), mName(name) {;}

HumanPlayer::~HumanPlayer(){
   ;
}

void HumanPlayer::InitMap(){
   for (auto &uptr: BoardClass::GetAllClasses()) {
      HumanPlayer::mNameMap[uptr->GetFriendlyName()] = uptr;
   }
}

//Checked: It works
void HumanPlayer::DoMove(){
   string line;
   list<unique_ptr<Board::Move>> lst;
   unique_ptr<Board::Move> oMove = mModel->GetCurBoard()->CreateMove();
   bool flag;

   try {
      flag = true;
      getline(cin, line);
      *oMove = line;
      mModel->GetCurBoard()->GetAllMoves(&lst);
      for (auto &uptr : lst) {
         if (*uptr == *oMove)
            try {
               mModel->MakeMove(move(oMove));
               flag = false;
               break;
            } catch (BaseException const &e) {
               throw BaseException(e);
            }                
      } 
      if (flag)
         throw BaseException(FString("Invalid move being applied"));
   } catch (BaseException const &e) {
      throw BaseException(e);
   }   
}

void HumanPlayer::ReadGame(){
   string filename;
   ifstream inputFile;
   inputFile.open(filename);
   mModel->Load(inputFile);
   inputFile.close();
}

void HumanPlayer::WriteGame(){
   string filename;
   ofstream outputFile;

   cin >> filename;
   outputFile.open(filename);
   mModel->Save(outputFile);
   outputFile.close();
}


void HumanPlayer::UndoMove(){
   if (mModel->TakeBack(mModel->GetPlayers().find(GetPosition())->second))
      cout << "Move undone." <<endl;
   else
      cout << "Opposing player(s) disagree; no undo." <<endl;
}

void HumanPlayer::GlobalNewGame(std::shared_ptr<GameModel> mdl, std::string gameName, int lvl, std::string *playerNames, int numPlayers){
   const BoardClass *myBoardClass;
   myBoardClass = mNameMap[gameName];
   mdl->NewGame(myBoardClass);
   HumanPlayer::SetLevel(lvl);

   for (int i = 0; i < numPlayers; i++)
      if (playerNames[i].compare("ai") == 0)
         mdl->AddPlayer(make_shared<AIPlayer>(mdl, ("AI " + i)), i);
      else 
         mdl->AddPlayer(make_shared<HumanPlayer>(mdl, playerNames[i]), i);
}

void HumanPlayer::NewGame(){
   int level;
   string playerNames[2], type;
   const BoardClass *myBoardClass;

   cin >> type >> level >> playerNames[0] >> playerNames[1];

   for (auto itr = mModel->GetPlayers().begin(); itr != mModel->GetPlayers().end(); itr++)
      if(itr->second != mModel->GetPlayers().find(GetPosition())->second && !(itr->second->IsGameReplaceOK())){
         cout << "Not all other players agree. Sorry."<<endl;
         return;
      }

   for (auto uptr : mModel->GetPlayers()){
      mModel->DropPlayer(uptr.second);
   }

   myBoardClass = mNameMap[type];
   mModel->NewGame(myBoardClass);
   HumanPlayer::SetLevel(level);
      for (int i = 0; i < 2; i++)
      if (playerNames[i].compare("ai") == 0)
         mModel->AddPlayer(make_shared<AIPlayer>(mModel, ("AI " + i)), i);
      else 
         mModel->AddPlayer(make_shared<HumanPlayer>(mModel, playerNames[i]), i);
}

void HumanPlayer::PrintHelp(){
   cout << 
      "Commands are: \n"
      "doMove move\n"
      "readGame filename\n"
      "writeGame filename\n"
      "undoMove\n"
      "newGame [Checkers|Othello|Pylos] level players\n"
      "printHelp\n"
      "autoPlay numFullMoves\n"
      "setOptions\n"
      "switchSides\n"
      "showMoves\n"
      "quit\n" 
   <<endl;
}

void HumanPlayer::AutoPlay(){
   int numTimes;
   cin >> numTimes;

   mModel->AddPlayer(make_shared<AIPlayer>(mModel, mName, numTimes, shared_this()), mPos);
   mActive = false;
}

void HumanPlayer::SetOptions(){
   const BoardClass *myBoardClass;
   const Class *myDlgClass;
   Dialog *myDlg = 0;
   void *rules;
   char resp;
   int newLevel;

   cout << "Modify current play level of " << HumanPlayer::GetLevel() << " [y/n]? ";
   cin >> resp;
   if (resp == 'y'){
      Dialog::ReadLimitInt(cin, cout, &newLevel, 1, 15, "Enter new level");
      HumanPlayer::SetLevel(newLevel);
   }

   myBoardClass = dynamic_cast<const BoardClass*>(mModel->GetCurBoard()
    ->GetClass());
   myDlgClass = myBoardClass->GetDlgClass();
   myDlg = dynamic_cast<Dialog *>(myDlgClass->NewInstance());
   rules = myBoardClass->GetOptions();
   if (myDlg->Run(cin, cout, rules))
      myBoardClass->SetOptions(rules);

   delete rules;
}

void HumanPlayer::SwitchSides(){
   mModel->SwapSides(shared_this());
}

void HumanPlayer::ShowMoves(){
   list<unique_ptr<Board::Move>> lst;
   list<shared_ptr<const Board::Move>> cLst;

   mModel->GetCurBoard()->GetAllMoves(&lst);
   for (auto &uptr : lst) {
      cLst.push_back(std::move(uptr));                        
   }
   PrintMoves(cLst);
}

// Implementations of Observer methods.
void HumanPlayer::Refresh(int flags){

}

// Handle all housekeeping for game-saving.  Return true if OK to clear Game
bool HumanPlayer::ClearGame(bool ask){
   char resp;
   string filename;

   if (ask){
      cout << "Current game is unsaved.  [S]ave/[C]ancel/[P]roceed? ";
      cin >> resp;

      if (resp == 'S'){
         cout << "Enter filename: ";
         cin >> filename;
         //NP: TODO: save the current game to the filename

      } else if (resp == 'C') {
         cout << "Current game left intact" <<endl;
         return false;
      } else if (resp == 'P'){
         //NP: It might be this or we might just return true, idk
         mModel->DropPlayer(shared_this());
      }
   } else 
      mModel->DropPlayer(shared_this());
   
   return true;
}

bool HumanPlayer::IsChangePlaceOK(){
   char resp;
   cout << mName << ", will you trade places [y/n]? ";
   cin >> resp;

   return resp == 'y';
}

bool HumanPlayer::IsTakeBackOK(){
   char resp;
   cout << mName << ", is it OK to take back the last move [y/n]? ";
   cin >> resp;

   return resp == 'y';
}

bool HumanPlayer::IsGameReplaceOK(){
   char resp;
   cout << mName << ", is it OK to replace the game [y/n]? ";
   cin >> resp;

   return resp == 'y';
}

void HumanPlayer::Step(){
   string command;
   int moveHistSize = 0;

   while (true){
      cout << "Enter command (for " << mName << "): ";
      cin >> command;
      cout <<endl;

      if (command.compare("doMove") == 0){
         try{
            DoMove();
            break;
         } catch (BaseException const &e) {
            cout << e.what() <<endl;
         } 
      } else if (command.compare("readGame") == 0){
         ReadGame();
      } else if (command.compare("writeGame") == 0){
         WriteGame();
      } else if (command.compare("undoMove") == 0){
         moveHistSize = mModel->GetCurBoard()->GetMoveHist().size();
         UndoMove();
         if (moveHistSize == 1)
            break;
      } else if (command.compare("newGame") == 0){
         NewGame();
      } else if (command.compare("printHelp") == 0){
         PrintHelp();
      } else if (command.compare("autoPlay") == 0){
         AutoPlay();
      } else if (command.compare("setOptions") == 0){
         SetOptions();
      } else if (command.compare("switchSides") == 0){
         SwitchSides();
      } else if (command.compare("showMoves") == 0){
         ShowMoves();
      } else if (command.compare("quit") == 0){
         ClearGame(mModel->IsChanged());
      }

      cout <<endl;
   }


   cin.clear();
}

void HumanPlayer::PrintMoves(const list<shared_ptr<const Board::Move>> &lst) {
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
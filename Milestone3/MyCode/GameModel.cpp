#include "GameModel.h"
using namespace std;

GameModel::GameModel() : mChanged(false), mPlayers(), mObservers(), mCurBoard
 (0) {;}


void GameModel::RefreshAllObservers(int flag) const{
   ObsList::iterator itr;

   for (itr = mObservers.begin(); itr != mObservers.end(); itr++)
      (*itr)->Refresh(flag);
}

void GameModel::NewGame(const BoardClass *cls){
   mChanged = true;

   Board *tempBoard = dynamic_cast<Board *>(cls->NewInstance());
   shared_ptr<Board> mB{move(tempBoard)};
   mCurBoard = dynamic_pointer_cast<Board>(mB);

   RefreshAllObservers(Observer::kClass);
}

void GameModel::MakeMove(unique_ptr<Board::Move> move){
   mChanged = true;

   mCurBoard->ApplyMove(move->Clone());
   RefreshAllObservers(Observer::kMove);
}

bool GameModel::TakeBack(shared_ptr<GameModel::Player> requestor){
   PlyMap::iterator itr;

   for (itr = mPlayers.begin(); itr != mPlayers.end(); itr++)
      if ((itr->second != requestor) && !(itr->second->IsTakeBackOK()))
         return false;
   mChanged = true;

   mCurBoard->UndoLastMove();
   RefreshAllObservers(Observer::kBoard);
   return true;
}

bool GameModel::SwapSides(shared_ptr<GameModel::Player> requestor){
   PlyMap::iterator itr;
   for (itr = mPlayers.begin(); itr != mPlayers.end(); itr++)
      if(itr->second != requestor && !(itr->second->IsChangePlaceOK())){
         cout << "Opposing player(s) disagree; no swap."<<endl;
         return false;
      }
   mChanged = true;
   for (itr = mPlayers.begin(); itr != mPlayers.end(); itr++){
      if (itr->second->GetPosition() == 0)
         itr->second->SetPosition(1);
      else
         itr->second->SetPosition(0);
   }
   RefreshAllObservers(Observer::kStatus);

   cout << "Sides swapped." <<endl;
   return true;
}

bool GameModel::IsPlayable(){
   const BoardClass *myBoardClass;
   myBoardClass = dynamic_cast<const BoardClass*>(mCurBoard->GetClass());
   int minPlayersNeeded = myBoardClass->GetMinPlayers();
   return minPlayersNeeded <= mPlayers.size();
}

 bool GameModel::IsDone() const{
   list<unique_ptr<Board::Move>> lst;
   mCurBoard->GetAllMoves(&lst);
   return lst.size() == 0;
 }

void GameModel::Load(istream &in){
   ;
}

void GameModel::Save(ostream &out) const{
   ;
}

void GameModel::AddPlayer(shared_ptr<GameModel::Player> ply, int pos){
   ply->SetPosition(pos);
   mPlayers[pos] = ply;
   mObservers.insert(ply);
   RefreshAllObservers(Observer::kStatus);
}

void GameModel::DropPlayer(shared_ptr<GameModel::Player> ply){
   PlyMap::iterator itr;
   for (itr = mPlayers.begin(); itr != mPlayers.end() && itr->second != ply; itr++){
      if (itr->second == ply){
         mPlayers.erase(itr);
         mObservers.erase(ply);
         RefreshAllObservers(Observer::kStatus);
         return;
      }
   }
}

shared_ptr<const GameModel::Player> GameModel::GetWhoseMove() const{
   return mPlayers.find(mCurBoard->GetWhoseMove())->second;
}






#include "AIPlayer.h"
#include <assert.h>

using namespace std;

int AIPlayer::mLevel = 10;

AIPlayer::AIPlayer(shared_ptr<GameModel> model, string name,
 int lim, shared_ptr<HumanPlayer> prx) : mModel(model), mName(name), mLimit(lim){
    mBook = new Book();
 }


void AIPlayer::Refresh(int flags){
   ;
}

bool AIPlayer::IsChangePlaceOK(){
   //NP : TODO: This if statement may be right, it may be wrong
   if (mModel->GetCurBoard()->GetValue() > 0 && mPosition == 0 || 
    mModel->GetCurBoard()->GetValue() < 0 && mPosition == 1){
       cout << "No thanks, I'm winning" <<endl;
       return false;
   } else {
      cout << "I think that's a good deal." <<endl;
      return true;
   }
      
}

// Make one move if it's your turn.  Quit the game if you finish it and
// quitAtEnd was true.  Kill yourself if you didn't quit the game, but
// you've hit your "limit".
void AIPlayer::Step(){
   mModel->MakeMove((FigureMove().bestMove)->Clone());
   if (mLimit-- <= 0 || mModel->IsDone())
      mModel->DropPlayer(shared_this());  
}

BestMove AIPlayer::FigureMove() const{
   unique_ptr<const Board::Key> key;
   const BoardClass *myBoardClass;
   Book::iterator itr;
   Book *tt = 0;
   BestMove bMove;
   if ((itr = mBook->find(key = mModel->GetCurBoard()->GetKey())) == mBook->end()){
      myBoardClass = dynamic_cast<const BoardClass*>(mModel->GetCurBoard()
    ->GetClass());
      if (myBoardClass->UseTransposition())
         tt = new Book();
      Minimax(mModel->GetCurBoard()->Clone().get(), mLevel, -Board::kWinVal - 1, Board::kWinVal + 1, &bMove, tt);
      (*mBook)[move(key)] = bMove;
      return bMove;
   } else {
      return itr->second;
   }
}

void AIPlayer::Minimax(Board *brd, int lvl, long min, long max, BestMove *res, Book *bk, int debugLvl){
   list<unique_ptr<Board::Move>> moves;
   list<unique_ptr<Board::Move>>::iterator mItr;
   BestMove subRes(nullptr, nullptr, 0, lvl, 1);
   unique_ptr<const Board::Key> key = brd->GetKey();
   Book::iterator itr;

   assert(lvl >= 1);

   if (bk && (itr = bk->find(key)) != bk->end()
    && lvl <= itr->second.depth) {
      *res = itr->second;
      res->numBoards = 1;
   }
   else {
      brd->GetAllMoves(&moves);
      *res = subRes;
      res->value = moves.size() == 0 ? brd->GetValue() :
       (brd->GetWhoseMove() ? Board::kWinVal + 1 : -Board::kWinVal - 1);

      for (mItr = moves.begin(); min < max && mItr != moves.end(); mItr++) {
         brd->ApplyMove((*mItr)->Clone());
         if (lvl == 1)
            subRes.value = brd->GetValue();
         else
            Minimax(brd, lvl-1, min, max, &subRes, bk, debugLvl);

         brd->UndoLastMove();

         if (brd->GetWhoseMove() == 0 && subRes.value > min) {
            res->value = min = subRes.value;
            res->SetBestMove((*mItr)->Clone());
            res->SetReply(subRes.bestMove);
         }
         else if (brd->GetWhoseMove() == 1 && subRes.value < max) {
            res->value = max = subRes.value;
            res->SetBestMove((*mItr)->Clone());
            res->SetReply(subRes.bestMove);
         }
         res->numBoards += subRes.numBoards;
      }

      if (bk && lvl >= SAVE_LEVEL && min < max && res->bestMove) {
         if ((itr = bk->find(key)) == bk->end())
            (*bk)[move(key)] = *res;
         else if (lvl > itr->second.depth)
            itr->second = *res;
      }
   }
}

void AIPlayer::SetLevel(int val){
   ;
   //NP: Level is static so you have to change the level on all of the AI Players

}
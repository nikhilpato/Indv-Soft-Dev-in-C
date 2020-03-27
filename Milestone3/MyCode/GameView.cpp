#include "GameView.h"
#include "View.h"

using namespace std;

GameView::GameView(std::shared_ptr<GameModel> mdl, std::ostream &out) : mModel(mdl), mOut(out) {}

void GameView::Refresh(int flags){
   if (flags & kClass){
      const BoardClass *myBoardClass;
      myBoardClass = dynamic_cast<const BoardClass*>(mModel->GetCurBoard()
       ->GetClass());
      
      mBrdView.reset(dynamic_cast<View *>
       (myBoardClass->GetViewClass()->NewInstance()));
      
      mBrdView->SetModel(mModel->GetCurBoard());

      PrintWin();
   }
   
   if (flags & kStatus || flags & kMove || flags & kBoard){
      PrintWin();
   }
}

void GameView::PrintWin(){
   string s;
   string temp;
   int width = 0;
   int numPlayers = mModel->GetPlayers().size();
   int boardVal = mModel->GetCurBoard()->GetValue();
   int i = 0;

   cout << "------" <<endl;
   
   for (auto const eachPlayer : mModel->GetPlayers())
      if (eachPlayer.second->GetName().size() > width)
         width = eachPlayer.second->GetName().size();

   for (auto const eachMove : mModel->GetCurBoard()->GetMoveHist())
      if (((string)*eachMove).size() > width)
         width = ((string)*eachMove).size();

   s = "Move  ";
   if (mModel->GetPlayers().size() > 0){
      temp = mModel->GetPlayers().find(0)->second->GetName();
      temp.resize(width, ' ');
   } else
      temp = "N/A";
   s += temp;
   s += "  ";
   if (mModel->GetPlayers().size() > 1){
      temp = mModel->GetPlayers().find(1)->second->GetName();
      temp.resize(width, ' ');
   } else
      temp = "N/A";
   s += temp;

   cout << s;
   s = "";

   for (auto const eachMove : mModel->GetCurBoard()->GetMoveHist()){
      if (i % numPlayers == 0){
         if (s.size() > 1) {
            s.pop_back();
            s.pop_back();
         }
         s += '\n';
         temp = to_string(i / numPlayers);
         temp.resize(4, ' ');
         s += temp;
         s += "  ";
      }

      temp = (string)*eachMove;
      temp.resize(width, ' ');
      s += temp;
      s += "  ";
      i++;
   }
   cout << s;

   cout << "\n\nCurrent Board:" <<endl;
   mBrdView->Draw(mOut);

   cout << "\nValue: " << boardVal <<endl <<endl;

   if (mModel->IsDone())
      mOut << (boardVal == Board::kWinVal ? "White" : boardVal == 
       -Board::kWinVal ? "Black" : "The game ended in a tie") <<std::endl;

   cout << "------" <<endl <<endl;
}
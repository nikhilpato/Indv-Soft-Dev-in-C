#include <fstream>
#include "MinimaxApp.h"
#include "GameView.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "MyLib.h"
 
using namespace std;

MinimaxApp MinimaxApp::mApp;

MinimaxApp::MinimaxApp() : mView(0), mModel(0) {
   mModel = make_shared<GameModel>();
   mView = make_shared<GameView>(mModel, cout);
   mModel->AddObserver(mView);
}

void MinimaxApp::Run() {
   int level;
   string players[2], type;

   HumanPlayer::InitMap();

   cout << "Enter game, level, player1, player2: ";
   cin >> type >> level >> players[0] >> players[1];
   HumanPlayer::GlobalNewGame(mModel, type, level, players, 2);

   while (Steppable::SteppablesExist())
      Steppable::StepAll();
}

int main(int argc, char **argv) 
{
   try {
      MinimaxApp::GetApp()->Run();
   } catch (BaseException &err) {
      cerr << "Error: " << err.what() << endl;
   }

   return 0;
}

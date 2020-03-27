#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include <string>
#include <memory>
#include "GameModel.h"
#include "GameView.h"
#include "Steppable.h"
#include "Dialog.h"
#include "View.h"

class HumanPlayer : public GameModel::Player, Steppable ,std::enable_shared_from_this<HumanPlayer> {
public:
   typedef std::map<std::string, const BoardClass *> NameMap;

   HumanPlayer(std::shared_ptr<GameModel> mdl, std::string name);
   ~HumanPlayer();

   void Reactivate()            {mActive = true;}

   // Return number of HumanPlayers
   static int GetCount() {return mCount;}

   static int GetLevel() {return mLevel;}
   static void SetLevel(int lvl) {mLevel = lvl;}
   
   static void InitMap();
      
   // Configure a brand new game in the specified GameModel
   static void GlobalNewGame(std::shared_ptr<GameModel> mdl, std::string gameName, 
    int lvl, std::string *playerNames, int numPlayers);

   std::shared_ptr<HumanPlayer> shared_this(){
      return shared_from_this();
   }

protected:

   // Handle each of the possible user-commands.
   virtual void DoMove();
   virtual void ReadGame();
   virtual void WriteGame();
   virtual void UndoMove();
   virtual void NewGame();
   virtual void PrintHelp();
   virtual void AutoPlay();
   virtual void SetOptions();
   virtual void SwitchSides();
   virtual void ShowMoves();

   // Implementations of Observer methods.
   virtual void Refresh(int flags) override;

   // Handle all housekeeping for game-saving.  Return true if OK to clear Game
   virtual bool ClearGame(bool ask);

   // Player Redefinitions
   void SetPosition(int pos)    override {mPos = pos;}
   int  GetPosition()           override {return mPos;}
   const std::string &GetName() override {return mName;}
   bool IsChangePlaceOK() override;
   bool IsTakeBackOK()    override;
   bool IsGameReplaceOK() override;
   
   // Steppable Redefinition
   void Step() override;
   void PrintMoves(const std::list<std::shared_ptr<const Board::Move>> &lst);

protected:
   // Member data as needed
   std::shared_ptr<GameModel> mModel;
   mutable bool mActive;
   static int mCount;
   int mPos;
   std::string mName;
   static NameMap mNameMap;
   static int mLevel;
};

#endif


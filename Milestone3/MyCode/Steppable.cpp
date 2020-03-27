#include "Steppable.h"

using namespace std;

set<Steppable *> Steppable::mRoster;

Steppable::Steppable(){
   mRoster.insert(this);
}

Steppable::~Steppable(){
   mRoster.erase(this);
}

// Call Step on all current Steppables.  Note that each Step call may
// alter the current Steppables list by addition or deletion.
void Steppable::StepAll(){
   set<Steppable *>::iterator itr;
   for (itr = mRoster.begin(); itr != mRoster.end(); itr++)
      (*itr)->Step();
}

bool Steppable::SteppablesExist(){
   return !(mRoster.empty());
}
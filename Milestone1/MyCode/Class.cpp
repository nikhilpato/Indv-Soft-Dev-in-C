#include "Class.h"

Class *Class::mClsHead;
BoardClass *BoardClass::mBrdClsHead;

Class::Class(const std::string &n, Object *(*c)()) : mName(n), mCreate(c), mNext
 (mClsHead) {
   mClsHead = this;
}

Object *Class::NewInstance() const{
   return (*mCreate)();
}

const Class *Class::ForName(const std::string &name) {
   Class *current = mClsHead;

   while (current && name.compare(current->mName)) {
      current = current->mNext;
   }
   return current;
}

BoardClass::BoardClass(
 const std::string &n, Object *(*c)(), const std::string &fn, const Class 
  *myViewClass, const Class *myDlgClass, void (*setOptions)(const void*), void *
   (*getOptions)(), bool useXPos, int minPlayers) : Class(n, c), mFriendlyName
    (fn), mViewClass(myViewClass), mDlgClass(myDlgClass), mSetOptions
     (setOptions), mGetOptions(getOptions), mUseXPos(useXPos), mMinPlayers
      (minPlayers), mNext(mBrdClsHead) {
    mBrdClsHead = this;
 }

void *BoardClass::GetOptions() const{
   return (*mGetOptions)();
}

void BoardClass::SetOptions(void *opts) const{
   (*mSetOptions)(opts);
}

std::vector<const BoardClass*> BoardClass::GetAllClasses() {
   std::vector<const BoardClass*> result;
   BoardClass *current = mBrdClsHead;

   while (current) {
      result.push_back(current);
      current = current->mNext;
   }

   return result;
   
}
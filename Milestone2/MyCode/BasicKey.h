#ifndef BasicKey_h
#define BasicKey_h


#include "MyLib.h"
#include "Board.h"

template <int size> class BasicKey : public Board::Key{
   public:
      BasicKey() {
         for (int i = 0; i < size; i++)
            vals[i] = 0;
      }
      virtual ~BasicKey() {}

      uint vals [size];

      const static Class mClass;
      const Class *GetClass() const { return &mClass;};

      virtual bool operator==(const Board::Key &rhs) const;
      virtual bool operator<(const Board::Key &rhs) const;

      void operator delete(void *p);
      void *operator new(size_t sz);

   protected:
      virtual std::istream &Read(std::istream &is);
      virtual std::ostream &Write(std::ostream &os) const;

      static Object *Create() {return new BasicKey<size>();}
      static std::vector<BasicKey<size> *> mFreeList;
};


template <int size> const Class BasicKey<size>::mClass(FString
 ("BasicKey<%d>",size), BasicKey<size>::Create);

template <int size> std::vector<BasicKey<size>*> BasicKey<size>::mFreeList;

template <int size> bool BasicKey<size>::operator==(const Board::Key &rhs) const
{
   const BasicKey &oRhs = dynamic_cast<const BasicKey &>(rhs);
   for (int i = 0; i < size; i++)
      if (vals[i] != oRhs.vals[i])
         return false;

   return true;
}

template <int size> bool BasicKey<size>::operator<(const Board::Key &rhs) const 
{
   const BasicKey &oRhs = dynamic_cast<const BasicKey &>(rhs);
   int i = 0;
   while (i < size) 
      if (vals[i] < oRhs.vals[i])
         return true;
      else if (vals[i] == oRhs.vals[i])
         i++;
      else
         return false;
   
   return false;
}

template <int size> std::istream &BasicKey<size>::Read(std::istream &is) {
   uint temp;
   for (int i = 0; i < size; i++){
      is.read((char *)&temp, sizeof(temp));
      vals[i] = EndianXfer(temp);
   }
   return is;
}

template <int size> std::ostream &BasicKey<size>::Write(std::ostream &os) const 
{
   uint temp;
   for (int i = 0; i < size; i++){
      temp = EndianXfer(vals[i]);
      os.write((char *)&temp, sizeof(temp));
   }
   return os;
}

template <int size> void *BasicKey<size>::operator new(size_t sz) {
   void *temp;

   if (mFreeList.size()) {
      temp = mFreeList.back();
      mFreeList.pop_back();
   } 
   else
      temp = ::new char[sz];

   Board::Key::mOutstanding++;
   return temp;
}

template <int size> void BasicKey<size>::operator delete(void *p) {
   mFreeList.push_back((BasicKey<size> *)p);
   Board::Key::mOutstanding--;
}

#endif
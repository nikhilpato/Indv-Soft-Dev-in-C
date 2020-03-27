#ifndef MYLIB_H
#define MYLIB_H

#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <exception>
#include <utility>

#pragma warning (disable: 4996)

// String subclass offering a printf-style constructor
class FString : public std::string {
public:
   FString(const char *fmt, ...) {
      static const int bufLen = 1024;
      static char buf[bufLen];
      va_list args;

      va_start(args, fmt);
      vsnprintf(buf, bufLen, fmt, args);
      va_end(args);

      *(std::string *)this = std::string(buf);
   }
};

// The consts in the parameter lists keep the compiler
// from griping about mismatches between const long and long, for instance.
// Note that Sqr is really no faster than pow(val, 2), just briefer
template <class T>
inline T Sqr(const T val) {return val*val;}

template <class T>
inline int InRange(const T lo, const T x, const T hi)
 {return !(x < lo) && x < hi;}

/* See also abs, clamp, min and max in algorithms. */

template <class Ptr>
struct LessPtr {
   int operator()(const Ptr &a, const Ptr &b) const {return *a < *b;} 
};

typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef const char *CStr;

#ifdef LITTLE_ENDIAN

inline ushort EndianXfer(ushort val) {return (val << 8) | (val >> 8);}
inline short  EndianXfer(short val)  {return (val << 8) | ((val >> 8) & 0xFF);}
inline int    EndianXfer(int val) {
   val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
   return (val << 16) | (val >> 16);   
}
inline uint   EndianXfer(uint val) {
   val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF); 
   return (val << 16) | ((val >> 16) & 0xFFFF);
}
inline long   EndianXfer(long val) {
   val = ((val >> 24) & 0xFF) | ((val >> 8) & 0xFF00); 
   return (val << 24) | ((val << 8) & 0xFF0000);  
}

#else

inline ushort EndianXfer(ushort val) {return val;}
inline short  EndianXfer(short val)  {return val;}
inline int    EndianXfer(int val)    {return val;}
inline uint   EndianXfer(uint val)   {return val;}
inline long   EndianXfer(long val)   {return val;}

#endif

class BaseException : public std::exception {
public:
   BaseException(const std::string &err) : mErr(err) {}
   ~BaseException() throw() {};
   const char *what() const throw() {return mErr.c_str();}

protected:
   std::string mErr;
};

#endif


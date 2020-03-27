#include "MancalaDlg.h"
#include "MancalaBoard.h"

using namespace std;

const Class MancalaDlg::mClass("MancalaDlg", MancalaDlg::Create);

bool MancalaDlg::Run(istream &in, ostream &out, void *data) {
  MancalaBoard::Rules *rules = reinterpret_cast<MancalaBoard::Rules *>(data);
   char resp;

   out << "Home weight: " << rules->homeWgt << endl
      << "Empty bowl weight: " << rules->emptBwlWgt << endl
      << "Move weight: " << rules->moveWgt << endl;

   out << endl << "Modify? [y/n] ";
   if ((in >> resp).eof())
      throw BaseException("Unexpected EOF");
   in.clear();
   in.ignore(256,'\n');
   if (resp == 'y') {
      out << endl;
      ReadLimitInt(in, out, &rules->homeWgt, 1, 1000, "Enter home weight");
      ReadLimitInt(in, out, &rules->emptBwlWgt, 1, 500,
       "Enter empty bowl weight");
      ReadLimitInt(in, out, &rules->moveWgt, 1, 500, "Enter move weight");
   }

   return resp == 'y';
}

#include "CheckersDlg.h"
#include "CheckersBoard.h"

using namespace std;

const Class CheckersDlg::mClass("CheckersDlg", CheckersDlg::Create);

bool CheckersDlg::Run(istream &in, ostream &out, void *data) {
  CheckersBoard::Rules *rules = reinterpret_cast<CheckersBoard::Rules *>(data);
   char resp;

   out << "(Piece weight is always 100)" << endl
      << "King weight: " << rules->kingWgt << endl
      << "Back row weight: " << rules->backWgt << endl
      << "Move weight: " << rules->moveWgt << endl;

   out << endl << "Modify? [y/n] ";
   if ((in >> resp).eof())
      throw BaseException("Unexpected EOF");
   in.clear();
   in.ignore(256,'\n');
   if (resp == 'y') {
      out << endl;
      ReadLimitInt(in, out, &rules->kingWgt, 0, 1000, "Enter king weight");
      ReadLimitInt(in, out, &rules->backWgt, 0, 1000, "Enter back-row weight");
      ReadLimitInt(in, out, &rules->moveWgt, 0, 100, "Enter move weight");
   }
   return resp == 'y';
}

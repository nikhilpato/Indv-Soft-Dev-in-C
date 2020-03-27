# General test makefile for multi-target test
# Copyright Clinton Staley 12/1/01


# General definitions
CPP = g++
CPPFLAGS = -std=c++11 -g -w -O3 -DLITTLE_ENDIAN

MANCALAOBJS = MancalaBoard.o MancalaMove.o MancalaView.o MancalaDlg.o
OTHELLOOBJS = OthelloBoard.o OthelloMove.o OthelloView.o OthelloDlg.o
CHECKERSOBJS = CheckersBoard.o CheckersMove.o CheckersView.o CheckersDlg.o
GAMEOBJS = Board.o Dialog.o Class.o $(OTHELLOOBJS) $(CHECKERSOBJS) $(MANCALAOBJS)
BOARDTESTOBJS = BoardTest.o $(GAMEOBJS)
MYBOARDTESTOBJS = MyBoardTest.o $(GAMEOBJS)
MAKEBOOKOBJS = MakeBook.o Book.o SimpleAIPlayer.o $(GAMEOBJS)
MINIMAXOBJS = MinimaxApp.o Steppable.o AIPlayer.o GameModel.o GameView.o HumanPlayer.o Book.o $(GAMEOBJS)

MinimaxApp : $(MINIMAXOBJS)
	$(CPP) $(MINIMAXOBJS) -o MinimaxApp

MakeBook : $(MAKEBOOKOBJS)
	$(CPP) $(MAKEBOOKOBJS) -o MakeBook

MyBoardTest : $(MYBOARDTESTOBJS)
	$(CPP) $(MYBOARDTESTOBJS) -o MyBoardTest

BoardTest : $(BOARDTESTOBJS)
	$(CPP) $(BOARDTESTOBJS) -o BoardTest

BTRelease: BoardTest BoardTest.o
	chmod 755 BoardTest
	chmod 644 BoardTest.o
	mv BoardTest BoardTest.o ../../milestone1

MBRelease: MakeBook
	chmod 755 MakeBook
	mv MakeBook ../../prj2

MMARelease: MinimaxApp
	cp MinimaxApp Pylos.gbk ../../prj3
	chmod 755 ../../prj3/MinimaxApp
	chmod 644 ../../prj3/Pylos.gbk

clean:
	rm BoardTest MyBoardTest *.o

# Buggy version dependencies and definitions
GAMEOBJSB0 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(OTHELLOOBJS) $(PYLOSOBJS) MancalaBoard.o MancalaMoveB0.o MancalaView.o MancalaDlg.o

GAMEOBJSB1 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(OTHELLOOBJS) $(PYLOSOBJS) MancalaBoardB1.o MancalaMove.o MancalaView.o MancalaDlg.o 

GAMEOBJSB2 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(OTHELLOOBJS) $(PYLOSOBJS) MancalaBoardB2.o MancalaMove.o MancalaView.o MancalaDlg.o 

GAMEOBJSB3 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(OTHELLOOBJS) $(PYLOSOBJS) MancalaBoardB3.o MancalaMove.o MancalaView.o MancalaDlg.o 

GAMEOBJSB4 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(OTHELLOOBJS) $(PYLOSOBJS) MancalaBoardB4.o MancalaMove.o MancalaView.o MancalaDlg.o 

GAMEOBJSB16 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(OTHELLOOBJS) $(PYLOSOBJS) MancalaBoard.o MancalaMoveB16.o MancalaView.o MancalaDlg.o 

GAMEOBJSB17 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(OTHELLOOBJS) $(PYLOSOBJS) MancalaBoard.o MancalaMoveB17.o MancalaView.o MancalaDlg.o 

GAMEOBJSB20 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(PYLOSOBJS) $(MANCALAOBJS) OthelloBoardB20.o OthelloMove.o OthelloView.o OthelloDlg.o

GAMEOBJSB21 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(PYLOSOBJS) $(MANCALAOBJS) OthelloBoardB21.o OthelloMove.o OthelloView.o OthelloDlg.o

GAMEOBJSB22 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(PYLOSOBJS) $(MANCALAOBJS) OthelloBoardB22.o OthelloMove.o OthelloView.o OthelloDlg.o

GAMEOBJSB23 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(PYLOSOBJS) $(MANCALAOBJS) OthelloBoardB23.o OthelloMove.o OthelloView.o OthelloDlg.o

GAMEOBJSB24 = BoardTest.o Board.o Dialog.o Class.o $(CHECKERSOBJS) $(PYLOSOBJS) $(MANCALAOBJS) OthelloBoardB24.o OthelloMove.o OthelloView.o OthelloDlg.o

GAMEOBJSB25 = BoardTest.o Board.o Dialog.o Class.o $(OTHELLOOBJS) $(PYLOSOBJS) $(MANCALAOBJS) CheckersBoardB25.o CheckersMove.o CheckersView.o CheckersDlg.o

GAMEOBJSB26 = BoardTest.o Board.o Dialog.o Class.o $(OTHELLOOBJS) $(PYLOSOBJS) $(MANCALAOBJS) CheckersBoardB26.o CheckersMove.o CheckersView.o CheckersDlg.o

GAMEOBJSB27 = BoardTest.o Board.o Dialog.o Class.o $(OTHELLOOBJS) $(PYLOSOBJS) $(MANCALAOBJS) CheckersBoardB27.o CheckersMove.o CheckersView.o CheckersDlg.o

GAMEOBJSB28 = BoardTest.o Board.o Dialog.o Class.o $(OTHELLOOBJS) $(PYLOSOBJS) $(MANCALAOBJS) CheckersBoardB28.o CheckersMove.o CheckersView.o CheckersDlg.o

GAMEOBJSB29 = BoardTest.o Board.o Dialog.o Class.o $(OTHELLOOBJS) $(PYLOSOBJS) $(MANCALAOBJS) CheckersBoardB29.o CheckersMove.o CheckersView.o CheckersDlg.o

GAMEOBJSB30 = BoardTest.o Board.o Dialog.o Class.o $(OTHELLOOBJS) $(PYLOSOBJS) $(MANCALAOBJS) CheckersBoardB30.o CheckersMove.o CheckersView.o CheckersDlg.o

GAMEOBJSB31 = BoardTest.o Board.o Dialog.o Class.o $(OTHELLOOBJS) $(PYLOSOBJS) $(MANCALAOBJS) CheckersBoard.o CheckersMoveB31.o CheckersView.o CheckersDlg.o

GAMEOBJSB32 = BoardTest.o Board.o Dialog.o Class.o $(OTHELLOOBJS) $(PYLOSOBJS) $(MANCALAOBJS) CheckersBoard.o CheckersMoveB32.o CheckersView.o CheckersDlg.o

MancalaMoveB0.o : MancalaMove.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG0 MancalaMove.cpp -o MancalaMoveB0.o

MancalaBoardB1.o : MancalaBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG1 MancalaBoard.cpp -o MancalaBoardB1.o

MancalaBoardB2.o : MancalaBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG2 MancalaBoard.cpp -o MancalaBoardB2.o

MancalaBoardB3.o : MancalaBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG3 MancalaBoard.cpp -o MancalaBoardB3.o

MancalaBoardB4.o : MancalaBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG4 MancalaBoard.cpp -o MancalaBoardB4.o

MancalaMoveB16.o : MancalaMove.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG16 MancalaMove.cpp -o MancalaMoveB16.o

MancalaMoveB17.o : MancalaMove.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG17 MancalaMove.cpp -o MancalaMoveB17.o

OthelloBoardB20.o : OthelloBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG20 OthelloBoard.cpp -o OthelloBoardB20.o

OthelloBoardB21.o : OthelloBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG21 OthelloBoard.cpp -o OthelloBoardB21.o

OthelloBoardB22.o : OthelloBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG22 OthelloBoard.cpp -o OthelloBoardB22.o

OthelloBoardB23.o : OthelloBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG23 OthelloBoard.cpp -o OthelloBoardB23.o

OthelloBoardB24.o : OthelloBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG24 OthelloBoard.cpp -o OthelloBoardB24.o

CheckersBoardB25.o : CheckersBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG25 CheckersBoard.cpp -o CheckersBoardB25.o

CheckersBoardB26.o : CheckersBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG26 CheckersBoard.cpp -o CheckersBoardB26.o

CheckersBoardB27.o : CheckersBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG27 CheckersBoard.cpp -o CheckersBoardB27.o

CheckersBoardB28.o : CheckersBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG28 CheckersBoard.cpp -o CheckersBoardB28.o

CheckersBoardB29.o : CheckersBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG29 CheckersBoard.cpp -o CheckersBoardB29.o

CheckersBoardB30.o : CheckersBoard.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG30 CheckersBoard.cpp -o CheckersBoardB30.o

CheckersMoveB31.o : CheckersMove.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG31 CheckersMove.cpp -o CheckersMoveB31.o

CheckersMoveB32.o : CheckersMove.cpp
	$(CPP) $(CPPFLAGS) -c -DBUG32 CheckersMove.cpp -o CheckersMoveB32.o


BoardTestB0 : $(GAMEOBJSB0)
	$(CPP)  $(GAMEOBJSB0) -o BoardTestB0

BoardTestB1 : $(GAMEOBJSB1)
	$(CPP)  $(GAMEOBJSB1) -o BoardTestB1

BoardTestB2 : $(GAMEOBJSB2)
	$(CPP)  $(GAMEOBJSB2) -o BoardTestB2

BoardTestB3 : $(GAMEOBJSB3)
	$(CPP)  $(GAMEOBJSB3) -o BoardTestB3

BoardTestB4 : $(GAMEOBJSB4)
	$(CPP)  $(GAMEOBJSB4) -o BoardTestB4

BoardTestB5 : $(GAMEOBJSB5)
	$(CPP)  $(GAMEOBJSB5) -o BoardTestB5

BoardTestB6 : $(GAMEOBJSB6)
	$(CPP)  $(GAMEOBJSB6) -o BoardTestB6

BoardTestB7 : $(GAMEOBJSB7)
	$(CPP)  $(GAMEOBJSB7) -o BoardTestB7

BoardTestB8 : $(GAMEOBJSB8)
	$(CPP)  $(GAMEOBJSB8) -o BoardTestB8

BoardTestB9 : $(GAMEOBJSB9)
	$(CPP)  $(GAMEOBJSB9) -o BoardTestB9

BoardTestB10 : $(GAMEOBJSB10)
	$(CPP)  $(GAMEOBJSB10) -o BoardTestB10

BoardTestB11 : $(GAMEOBJSB11)
	$(CPP)  $(GAMEOBJSB11) -o BoardTestB11

BoardTestB12 : $(GAMEOBJSB12)
	$(CPP)  $(GAMEOBJSB12) -o BoardTestB12

BoardTestB13 : $(GAMEOBJSB13)
	$(CPP)  $(GAMEOBJSB13) -o BoardTestB13

BoardTestB14 : $(GAMEOBJSB14)
	$(CPP)  $(GAMEOBJSB14) -o BoardTestB14

BoardTestB15 : $(GAMEOBJSB15)
	$(CPP)  $(GAMEOBJSB15) -o BoardTestB15

BoardTestB16 : $(GAMEOBJSB16)
	$(CPP)  $(GAMEOBJSB16) -o BoardTestB16

BoardTestB17 : $(GAMEOBJSB17)
	$(CPP)  $(GAMEOBJSB17) -o BoardTestB17

BoardTestB18 : $(GAMEOBJSB18)
	$(CPP)  $(GAMEOBJSB18) -o BoardTestB18

BoardTestB19 : $(GAMEOBJSB19)
	$(CPP)  $(GAMEOBJSB19) -o BoardTestB19

BoardTestB20 : $(GAMEOBJSB20)
	$(CPP)  $(GAMEOBJSB20) -o BoardTestB20

BoardTestB21 : $(GAMEOBJSB21)
	$(CPP)  $(GAMEOBJSB21) -o BoardTestB21

BoardTestB22 : $(GAMEOBJSB22)
	$(CPP)  $(GAMEOBJSB22) -o BoardTestB22

BoardTestB23 : $(GAMEOBJSB23)
	$(CPP)  $(GAMEOBJSB23) -o BoardTestB23

BoardTestB24 : $(GAMEOBJSB24)
	$(CPP)  $(GAMEOBJSB24) -o BoardTestB24

BoardTestB25 : $(GAMEOBJSB25)
	$(CPP)  $(GAMEOBJSB25) -o BoardTestB25

BoardTestB26 : $(GAMEOBJSB26)
	$(CPP)  $(GAMEOBJSB26) -o BoardTestB26

BoardTestB27 : $(GAMEOBJSB27)
	$(CPP)  $(GAMEOBJSB27) -o BoardTestB27

BoardTestB28 : $(GAMEOBJSB28)
	$(CPP)  $(GAMEOBJSB28) -o BoardTestB28

BoardTestB29 : $(GAMEOBJSB29)
	$(CPP)  $(GAMEOBJSB29) -o BoardTestB29

BoardTestB30 : $(GAMEOBJSB30)
	$(CPP)  $(GAMEOBJSB30) -o BoardTestB30

BoardTestB31 : $(GAMEOBJSB31)
	$(CPP)  $(GAMEOBJSB31) -o BoardTestB31

BoardTestB32 : $(GAMEOBJSB32)
	$(CPP)  $(GAMEOBJSB32) -o BoardTestB32

AllBugs : BoardTestB0 BoardTestB1 BoardTestB2 BoardTestB3 BoardTestB4 BoardTestB5 BoardTestB6 BoardTestB7 BoardTestB8 BoardTestB9 BoardTestB10 BoardTestB11 BoardTestB12 BoardTestB13 BoardTestB14 BoardTestB15 BoardTestB16 BoardTestB17 BoardTestB18 BoardTestB19 BoardTestB20 BoardTestB21 BoardTestB22 BoardTestB23 BoardTestB24 BoardTestB25 BoardTestB26 BoardTestB27 BoardTestB28 BoardTestB29 BoardTestB30 BoardTestB31 BoardTestB32
	mv BoardTestB* /home/grade305/milestone0/turnin/tests

# Header file interdependencies

CheckersDlg.h OthelloDlg.h MancalaDlg.h: Dialog.h
	touch MancalaDlg.h CheckersDlg.h OthelloDlg.h

MancalaView.h OthelloView.h CheckersView.h : View.h
	touch MancalaView.h othelloView.h CheckersView.h

MancalaBoard.cpp: Class.h Board.h
	touch MancalaBoard.cpp
MancalaMove.cpp: MancalaBoard.h Class.h Board.h
	touch MancalaMove.cpp
MancalaView.cpp: MancalaBoard.h View.h Class.h Board.h
	touch MancalaView.cpp
MancalaDlg.cpp: MancalaBoard.h Dialog.h Class.h
	touch MancalaDlg.cpp

OthelloBoard.cpp: Class.h Board.h
	touch OthelloBoard.cpp
OthelloMove.cpp: OthelloBoard.h Class.h Board.h
	touch OthelloMove.cpp
OthelloView.cpp: OthelloBoard.h View.h Class.h Board.h
	touch OthelloView.cpp
OthelloDlg.cpp: OthelloBoard.h Dialog.h Class.h
	touch OthelloDlg.cpp

CheckersBoard.cpp: Class.h Board.h
	touch CheckersBoard.cpp
CheckersMove.cpp: CheckersView.h CheckersBoard.h  Board.h
	touch CheckersMove.cpp
CheckersView.cpp: CheckersBoard.h View.h Class.h  Board.h
	touch CheckersView.cpp
CheckersDlg.cpp: CheckersBoard.h Dialog.h Class.h 
	touch CheckersDlg.cpp

BoardTest.cpp: Board.h View.h Dialog.h

%.cpp: %.h
	touch $<

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

# DO NOT DELETE

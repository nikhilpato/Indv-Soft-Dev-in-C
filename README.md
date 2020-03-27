# Indv-Soft-Dev-in-C++
Instructor: Clint Staley

Project moved through building three board games (Othello, Checkers, and Mancala) in order to eventually make a full
commandline game in which users can play against other players or an AI


Milestone1:
In Milestone1 were were given the starter code for the Othello game with bugs in it which we had to find and fix ourselves.
The code for Checkers was partially given and no code for the Mancala game was given.  We also were required to implement a
reflection mechanism that would assume a generic board was selected and based on the commandline arguments it would 
dynamically cast the correcty Board, Move, Dialog, and View objects for the specified game.
Executable: BoardTest

Milestone2:
In Milestone2, the object was to implement a Book of moves that the AI can use for future moves.  The algorithm used to
generate the best moves in each case was Minimax with alpha beta pruning.  For Checkers, a transoposition table was used while
Othello and Mancala did not require one.
Executable: MakeBook

Milestone3:
In Milestone 3, everything was put together in a packaged product that could easily be played by a lay user.  The interface
was cleaned up and the program allowed for person-to-person play, person-to-AI play, or AI-to-AI play.  The Player can also
have an AI take over for a specified number of moves and will return control to the human player once that number of moves
have been completed.
Executable: MinimaxApp

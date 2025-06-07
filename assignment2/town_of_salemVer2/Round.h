#ifndef ROUND_H   //header file's guards
#define ROUND_H

#include "std_lib_facilities.h"

class Round{                                 //Here is the definition of class:'Round'
    
    string player_killed_from_gangster;   //contains the name of the player killed from gangster for this round
    int round_number;                     //contains the number of this particular round 
    string player_thrown_out_from_voting; //contains the name of the player who was voted (in this round) to leave the game

public:
    //Declarations of the class' constructors
    Round(string xx, int yy, string zz);
    Round();

    //Declarations for the class' 'getters' functions (we'll need them in the end for writing
    //into the file named "TownOfSalem_output.txt".
    string get_player_killed_from_gangster();        
    int get_round_number();
    string get_player_thrown_out_from_voting();
};

#endif //ROUND_H
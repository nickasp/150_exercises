#include "Round.h" 

//Class 'Round''s constructors' definitions
Round::Round(string xx, int yy, string zz)     //This constructor is for initializing the data members
    :player_killed_from_gangster(xx), round_number(yy), player_thrown_out_from_voting(zz){}

Round::Round(){}    //This constructor is for letting us declare an object of type 'Round' without
                    //having to initialize its data members.

string Round::get_player_killed_from_gangster(){
        return Round::player_killed_from_gangster;
}

int Round::get_round_number(){
    return Round::round_number;
}

string Round::get_player_thrown_out_from_voting(){
    return Round::player_thrown_out_from_voting;
}
#ifndef PLAYER_H
#define PLAYER_H

#include "std_lib_facilities.h"

class player{
private:
    // Class' data members' declarations
    string Name;   
    bool Alive;
    string Role;
    int CurrentVotes;

    /*
    Function check() checks the validity of the values given to it as parameters.
    void check_n(string);
    void check_a(bool);
    void check_r(string);
    */
public:
    // Class' "getters"' declarations 
    string get_Name();
    bool get_Alive();
    string get_Role();
    int get_CurrentVotes();

    // Class' "setters"' declarations
    void set_Name(string name);
    void set_Alive(bool alive);
    void set_Role(string role);
    void set_CurrentVotes(int currentvotes);

    /* Class' function "player_vote()"'s declaration. This function is responsible
    for the voting procedure of this particular player.*/
    void player_vote(vector<player> &players, int &total_number_of_players, int times, int count, vector<int> &p);

    // Class' constructors' declarations
    player(string Name, bool isAlive, string Role);
    player(); 
};

#endif //PLAYER_H
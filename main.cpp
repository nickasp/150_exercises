#include "std_lib_facilities.h"
#include "Round.h"
#include "player.h"

int main(){
    string file_name;
    cout << "Type the name of the file you want to open for reading: ";
    cin >> file_name;
    ifstream ist(file_name.c_str());
    if(!ist){
        cerr << "Runtime_Error: System could not  open file!\n";
        return 1;         
    }
    vector<player> players;
    int total_number_of_players = fill_vector_players(players, ist);

}

int fill_vector_players(vector<player> &players, ifstream &ist){
    string name, role;
    int n = 0;
    while(ist >> name >> role){
        players.push_back(player(name, true, role));
        n++;
    }
    if(ist.eof()){
        return n;
    }
}
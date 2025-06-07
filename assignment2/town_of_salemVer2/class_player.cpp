#include "player.h"
#include "errors.h"

// Class' setters' definitions
void player::set_Name(string name){
    string Name = name;
}
void player::set_Alive(bool alive){
    player::Alive = alive;
}
void player::set_Role(string role){
    player::Role = role;
}
void player::set_CurrentVotes(int currentvotes){
    player::CurrentVotes = currentvotes;
}

//Class' getters' definitions
string player::get_Name(){
    return player::Name;
}
bool player::get_Alive(){
    return player::Alive;
}
string player::get_Role(){
    return player::Role;
}
int player::get_CurrentVotes(){
    return player::CurrentVotes;
}

//Class' function player_vote() definition
void player::player_vote(vector<player> &players, int &total_number_of_players, int times, int count, vector<int> &p){
    int x;
    int j = 0;
    do{
        try{
            cin >> x;
            if(x<0 || x>(total_number_of_players-1)){
                throw wrong_number();
            }else if(players[x].get_Alive() == false){
                throw player_already_out();
            }else if(players[x].get_Alive() == true && times == 2){
                int flag = 0;
                for(int i=0; i<count; i++){
                    if(x == p[i]){
                        flag = 1;
                    }
                }
                if(flag == 0){
                    throw not_one_of_those_with_the_maniest_votes();
                }
            }
            else{
                players[x].set_CurrentVotes((players[x].get_CurrentVotes())+1);
                j = 1;  
            }                             
        }catch(wrong_number){
            cout << "INVALID NUMBER! PLEASE GIVE A NUMBEER BETWEEN 0 AND " << total_number_of_players
                 << ", WHICH WILL ALSO REFER TO A PLAYER WHO HAS NOT ALREADY LEFT THE GAME:\n";
        }catch(player_already_out){
            cout << "THIS PLAYER IS ALREADY OUT OF THE GAME. PLEASE CHOOSE ANOTHER PLAYER AND GIVE HIS NUMBER:\n";
        }catch(not_one_of_those_with_the_maniest_votes){
            cout << "YOU CAN NOT VOTE THIS PLAYER. THE VOTING NOW IS BETWEEN ONE OF THE FOLLOWING PLAYERS:\n";
            for(int i=0; i < count; i++){
                cout << "player " << players[i].get_Name() << " (player no. " << i << ")   ";
            }
            cout << "\n\nSO PLEASE VOTE AGAIN:\n";
        }
    }while(!j);
}

//Class' constructors' definitions
player::player(string Name, bool isAlive, string role)
    :Name(Name), Alive(isAlive){
        if(role[0] == 'g' || role[0] == 'G'){
            player::set_Role("gangster");
        }else if(role[0] == 'd' || role[0] == 'D'){
            player::set_Role("doctor");
        }else{
            player::set_Role("citizen");
        }
    }                                                                          
    
                                             

player::player(){}
                                      //This empty constructor is to enable us to declare 
                                      //an object of type "player" without being obligated to
                                      //initialize its data members.
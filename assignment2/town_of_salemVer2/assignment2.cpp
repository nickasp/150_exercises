#include "std_lib_facilities.h"  
#include "Round.h"   //.h file with which we include class 'Round' and its functions to our programme
#include "player.h"  //.h file with which we include class 'player' and its functions to our programme
#include "errors.h"  //.h  file with which we include the classes used as exceptions into our programme

/*HERE ARE THE DECLARATIONS OF ALL THE FUNCTIONS USED IN OUR PROGRAMME*/
void choice_menu(vector<player> &);
int night(int, vector<player>&);
void day(vector<player> &);
void voting(vector<player> &, int, int);
int max(vector<player> &);
void voting_results_check(vector<player> &, int);
void fill_vector_rounds(vector<player> &, int, int, int, vector<Round> &);
int check_for_end_of_game(vector<player> &,int);
int fill_vector_players(vector<player> & players, ifstream & ist);
int num_of_pl_thrown_out_from_voting;



int A = -1;   /* Here we will be storing the number of the last player that left the game. We initialize it with
                 number -1 because we don't want the compiler to give it number 0 by default (because there is a player
                 with this number) So what is this variable's role ? Well, each time a particular player has to leave the game,
                 variable A is given the number of this player. It is used in choice_menu() function when the user chooses either
                 option 3 (to check the case in which nobody has left the game yet), or option 4 (to check the case in which nobody 
                 has left the game yet or in order to answer the question who was the last player to leave the game).*/ 

int total_number_of_players;    //how many are the players in the beginning of the game
int current_number_of_players;  //how many players are into the game at any given moment 
int x1;                         //gangster's number
vector<int> p;                  //used later ,only if needed, in case the voting procedure has to be repeated in the same round 


/*MAIN FUNCTION*/
int main(){   
    /*In the beginning of our programme, the user types in the name of the file he/she wants to open
    for reading. This file is going to have the names of the players of the game and their roles.
    In case an error occurs and the system is unable to open the file, an error message will be displayed
    onto the screen and our programme will be terminated with exit code 1.*/
    string file_name;
    cout << "Type the name of the file you want to open for reading: ";
    cin >> file_name;   //  the user types in the name of the file
    ifstream ist(file_name.c_str());  // the system opens the file for reading
    if(!ist){                         //in case something goes wrong
        cerr << "Runtime_Error: System could not  open file!\n";  // an error message is shown
        return 1;                     // and the programme is terminated
    }
    vector<player> players;   //this is a vector with information for each player (name, alive/dead, role)
    vector<Round> round;      // a vector with information for each game round (name of player killed by the gangster, name of player
                              //thrown outside the game after voting procedure and number of round (e.g. 2nd round, 3rd round e.t.c.))
    total_number_of_players = fill_vector_players(players, ist);
    ist.close();              // close the file
    current_number_of_players = total_number_of_players; 
    int num_goodbye;
    int b;                    // We will use this variable as a way to escape or remain into the do-while loop.
    int num_of_round = 0;     //keep track of the number of rounds
    for(int i=0; i<total_number_of_players; i++){
        if(players[i].get_Role() == "gangster"){
            x1 = i;  // find the gangster and give his number to variable x1
            break;
        }
    }
    do{
        choice_menu(players); // We want to show the menu each time the day zone ends, before the new night zone begins.
        num_of_round++;      
        num_goodbye = night(x1, players);   // The NIGHT ZONE starts!
        if(num_goodbye != x1 && current_number_of_players > 2){
            day(players);         // The DAY ZONE starts, after the night zone has reached to an end!
        }
        fill_vector_rounds(players, num_of_round, num_goodbye, num_of_pl_thrown_out_from_voting, round);
        b = check_for_end_of_game(players,x1); /* This function is called to 'tell' us if the game has reached to an end or not. 
                                             It returns number 1 if the game has to continue , or number 0 if the game has to be finished.*/
    }while(b);         
    
    /*When the game has reached to an end, create a new file called "TownOfSalem_output.txt" 
    and write in it for each round of the game (line 1 --> 1st round, line 2 --> 2nd round ...) the name of the player who was killed by the gangster ,
    the number of that specific round and the name of the player thrown outside the game after the voting procedure.*/
    string name2 = "TownOfSalem_output.txt";
    ofstream ost(name2.c_str());   //open the file
    if(!ost){   //in case of error , print the message shown below and end programme with exit code 1
        cerr << "Runtime_Error: System could not create file 'TownOfSalem_output.txt'\n";
        return 1;         
    }
    int k = 0;
    do{
        ost <<" "<<round[k].get_player_killed_from_gangster() << round[k].get_round_number()<< " " << round[k].get_player_thrown_out_from_voting() << '\n';
        k++;
    }while(ost && k<num_of_round);
    ost.close();    //close the file

    return 0;
}                    

/* CHOICE_MENU FUNCTION */
/*This function, when it is called by main function, shows a menu with 5 different options available for the user/player to choose from.
The player chooses the option he wants and gets the associated result as a message on the screen. This process is repeated again and again,
until the player chooses option 5, function choice_menu() returns void and the game continues as it is. The player has to give as an input 
one number between number 1, 2, 3, 4 or 5, each time the menu shows up. If he gives a number lesser than 1 or greater than 5, error 
'wrong_number' occurs.*/
void choice_menu(vector<player> &players){
    int choice; 
    cout << "1) SHOW WHICH PLAYERS ARE STILL IN THE GAME\n"
         << "2) SHOW WHICH PLAYERS ARE STILL IN THE GAME AND WHAT ARE THEIR ROLES\n"
         << "3) SHOW WHICH PLAYERS HAVE ALREADY LEFT THE GAME\n"
         << "4) SHOW WHICH PLAYER WAS THE LAST ONE TILL NOW TO LEAVE THE GAME\n"
         << "5) EXIT MENU AND CONTINUE THROUGH THE GAME\n\n\n"
         << "select one of the above options and type its number:\n\n";
    do{
        cin >> choice;
        try{
            if(choice < 1 || choice > 5){
                throw wrong_number();
            }else if(choice == 1){
                cout << "STILL IN THE GAME ARE PLAYERS:\n";
                for(int i=0; i<total_number_of_players; i++){
                    if(players[i].get_Alive() == true){ //if player no. i is alive, print his name
                        cout << players[i].get_Name() <<" (player no." << i << ")\n";
                    }
                }
                cout << "\n\nselect one of the above options and type its number:\n\n";
            }else if(choice == 2){
                cout << "STILL IN THE GAME ARE PLAYERS:\n";
                for(int i=0; i<total_number_of_players; i++){
                    if(players[i].get_Alive() == true){  //if player no. i is alive, print his name
                        cout << players[i].get_Name() << " (player no." << i << ") --->" << players[i].get_Role() << '\n';
                    }
                }
                cout << "\n\nselect one of the above options and type its number:\n\n";
            }else if(choice == 3){
                if(A == -1){  
                    cout << "NOBODY HAS LEFT THE GAME YET.\n"; // because that's the only case var A's value could be no. 1
                }else{
                    cout << "THE PLAYERS WHO HAVE ALREADY LEFT THE GAME ARE PLAYERS:\n";
                    for(int i=0; i<total_number_of_players; i++){
                        if(players[i].get_Alive() == false){  //if player no. i is dead
                            cout << players[i].get_Name() << " (player no " << i << ")\n"; // print his name
                        }
                    }
                }
                cout << "\n\nselect one of the above options and type its number:\n\n";
            }else if(choice == 4){
                if(A == -1){
                    cout << "NOBODY HAS LEFT THE GAME YET.\n";
                }else{
                    cout << "THE PLAYER WHO WAS THE LAST ONE TILL NOW TO LEAVE THE GAME IS PLAYER: ";
                    cout << players[A].get_Name() << " (player no " << A << ")\n\n" //print player[A]'s name
                         <<"select one of the above options and type its number:\n\n";
                }   
            }else{
                cout << "OK, LET'S MOVE ON ...\n";
            }
        }catch(wrong_number){
            cout << "WRONG CHOICE. TRY AGAIN:\n";
        }         
    }while(choice != 5); //in case user types in no. 5, exit the do-while loop and let the new
}                        //round start!



// NIGHT ZONE begins!
int night(int x1, vector<player> &players){  
    cout <<"NIGHT ZONE BEGINS NOW!\nCITIZENS CLOSE YOUR EYES AND SLEEP!\n" 
        << "THE  GANGSTER IS PLAYER NUMBER " << x1 << "AND HIS NAME IS "<< players[x1].get_Name() <<"\n\n"
        << "GANGSTER GIVE THE NUMBER OF THE PLAYER YOU WANT TO LEAVE THE GAME:";
    int number_goodbye;
    int k = 0;
    do{                                    // We use an exception to 'catch' bad / wrong inputs.
        cin >> number_goodbye;             // we store the number of the player who the Gangster wants to leave   
        try{                               // the game.
            if(number_goodbye < 0 || number_goodbye > (total_number_of_players-1)){//If Gangster gave the number of a player who does
                throw wrong_number();                                             // not exist ---> wrong_number error!
            }else if(players[number_goodbye].get_Alive() == false){// If Gangster gave the number of a player who
                throw player_already_out();                        // has already left the game ---> player_already_out error!
            }else{
                k = 1;  // k's value has to differ from zero (0) in order to 'escape' the loop and let the game continue.
            }
        }catch(wrong_number){
            cout << "INVALID NUMBER! PLEASE GIVE A NUMBEER BETWEEN 0 AND "<< (total_number_of_players-1) << ", WHICH WILL ALSO REFER TO A PLAYER"
                 << " WHO HAS NOT ALREADY LEFT THE GAME:\n"; // In case of wrong_number error, print these messages.
        }catch(player_already_out){
            cout << "THIS PLAYER IS ALREADY OUT OF THE GAME. PLEASE CHOOSE ANOTHER PLAYER AND GIVE HIS NUMBER:\n";
        }                                             // In case of player_already_out error, print the message above.
    }while(!k);    // Check if there was an error or not and decide if the game will continue or if the gangster has to give another value.
    for(int i=0; i<total_number_of_players; i++){
        if(players[i].get_Role() == "doctor" && players[i].get_Alive() == true){ //ONLY IF doctor is still alive! 
            int k2 = 0;
            cout << "\n\nDOCTOR GIVE THE NUMBER OF THE PLAYER YOU ARE GOING TO 'VISIT' TONIGHT: ";
            int number_visit;
            do{
                cin >> number_visit;  // With the exact same way we check the validation of 
                try{                 // the value / number that doctor has given to us.
                    if(number_visit < 0 || number_visit > (total_number_of_players-1)){
                        throw wrong_number();
                    }else if(players[number_visit].get_Alive() == false){
                        throw player_already_out();
                    }else if(players[number_visit].get_Role() == "doctor"){
                        throw doc_cannot_visit_himself();
                    }
                    else{
                        k2 = 1;  
                    }
                }catch(wrong_number){
                    cout << "\n\nINVALID NUMBER! PLEASE GIVE A NUMBEER BETWEEN 0 AND " << (total_number_of_players-1) 
                         << ", WHICH WILL ALSO REFER TO A PLAYER WHO HAS NOT ALREADY LEFT THE GAME:\n";
                }catch(player_already_out){
                    cout << "\n\nTHIS PLAYER IS ALREADY OUT OF THE GAME. PLEASE CHOOSE ANOTHER PLAYER AND GIVE HIS NUMBER:\n";
                }catch(doc_cannot_visit_himself){
                    cout << "\n\nHEY DOCTOR, YOU CANNOT 'VISIT' YOURSELF! I'M SORRY!\nPLEASE, VOTE AGAIN!:\n";
                }
            }while(!k2);
            if(number_goodbye == number_visit){ // In case the player Gangster wants to leave and the player Doctor wants to visit
                cout << "\n\nNIGHT ZONE IS FINALLY OVER AND NOBODY IS LEAVING THE GAME!\n\n"; // are the same, nobody is leaving the game.
                return -1;
            }
            break;
        }
    }
    //In case the gangster decides to kill himself, the citizens win and the game is over!
    if(number_goodbye == x1){   
        cout << "THE GANGSTER COMMITED SUICIDE, SO THE GAME HAS REACHED TO AN END AND THE CITIZENS HAVE WON!\n";
        players[x1].set_Alive(false);  // 'write down' that player no. x1 has just died
        current_number_of_players -=1;  
        return x1;   // The return value is gangster's number
    }
    cout << "\n\nNIGHT ZONE IS FINALLY OVER, WITH PLAYER " << players[number_goodbye].get_Name() << " (player no " << number_goodbye 
         << ") LEAVING THE GAME.\n\n";
    players[number_goodbye].set_Alive(false);      // In other case, the player with the number the gangster has given leaves the 
    A = number_goodbye;                            // game and his number is given to variable A.
    current_number_of_players -= 1;                  // A player is leaving, so variable current_number_of_players has to be decreased by one.                    
    return number_goodbye;
}                                                  // Finally ,the NIGHT ZONE is over.

/*DAY FUNCTION*/
/*After the night zone is over, DAY ZONE begins!*/
void day(vector<player> &players){
    cout << "DAY ZONE BEGINS NOW!\n\n"
         << "IT'S TIME FOR VOTING!\n\n\n";
    cout << "THE REMAINING PLAYERS ARE:\n";
    for(int i=0; i<total_number_of_players; i++){    
        if(players[i].get_Alive() == true){         //check if player no. i is alive
            cout << players[i].get_Name() << " (player no. " << i << ")\n"; //and if so, print his name
        }
    }
    cout << '\n';
    //int times = 0;
    voting(players, 1, 0); //call function voting() for the first time and let the voting procedure begin!
}


/*VOTING FUNCTION*/
/*This function handles the voting procedure.*/
void voting(vector<player> &players, int times, int count){
    for(int i=0; i<total_number_of_players; i++){
        players[i].set_CurrentVotes(0);   //sets CurrentVotes with number 0 for each player
    }
    for(int i=0; i<total_number_of_players; i++){
        if(players[i].get_Alive() == true){
            cout << "PLAYER " << i << "PLEASE VOTE: "; 
            players[i].player_vote(players, total_number_of_players, times, count, p);
        }   //calls function player::player_vote() for each alive player and he/she votes 
    }
    cout << "THE VOTING PROCEDURE IS OVER!\n";
    voting_results_check(players, times); //calls voting_results_check() function in order to calculate the votes
                                 //each player has gathered and reach to the final result of the voting procedure.
 }


/*VOTING_RESULTS_CHECK FUNCTION*/
/*When called for the first time (from function voting()), it takes number 1 as its third argument. Instead, if it is called for the second time,
it takes number 2 as its third argument. This function calls function max() and we get what was the biggest 
number of votes that have been collected through the previously finished voting procedure. Then, we count how many players have collected that 
number of votes into the variable count. If we find out that it was only one player (if(count == 1)), he has to leave the game. The current_number_of_players 
is decreased by one and the player's number is given to variable A. If we find out that all players have collected the exact same amount of votes
(if(count == total_pl_num)), nobody is leaving. In any other case, one of two things happen. If function voting_results_check()'s third argument holds number 1 (if(times == 1)),
the voting procedure has to be repeated. For that reason, we call function voting() again, giving this time number 2 as its 2nd argument. But this time,applicants for exiting the 
game are going to be only those players who were even in total amount of votes. Instead, if voting_results_check()'s third argument holds number 2 (if(times == 2)), the voting 
procedure is terminated and no player leaves the game, because we will have reached to a point when for the second time in a row, more than one players will have collected the 
same amount of 'max' votes.*/
void voting_results_check(vector<player> &players, int times){
    int m = max(players);   // function max() returns the biggest number of votes that a player has collected in this round.
    int count = 0;
    for(int i=0; i<total_number_of_players; i++){
        if(players[i].get_CurrentVotes() == m){
            count += 1;       // we check if there are more than one players who have collected the biggest number of votes
        }
    }
    if(count == 1){
        for(int i=0; i<total_number_of_players; i++){
            if(players[i].get_CurrentVotes() == m){
                cout << "PLAYER " << i << " HAS COLLECTED THE MOST VOTES, SO HE HAS TO LEAVE THE GAME!\n\n";
                players[i].set_Alive(false);
                A = i;
                current_number_of_players -= 1;  // A player is leaving so the value of the variable total_pl_num has to be decreased by one.
                num_of_pl_thrown_out_from_voting = i;               
                return;
            }
        }
    }else if(count == current_number_of_players){  // In case all the remaining players (current_number_of_players) have collected the same amount of votes.
        cout << "ALL PLAYERS HAVE COLLECTED THE SAME NUMBER OF VOTES. THIS IS WHY NOBODY LEAVES THE GAME FOR THIS ROUND.\n";
        num_of_pl_thrown_out_from_voting = -1;
        return;
    }else{
        if(times == 2){ 
            cout << "THERE IS NOT ONE PLAYER WITH THE MANIEST VOTES FOR THE SECOND TIME IN A ROW, SO THIS IS WHY\n"
                 << "THE VOTING PROCEDURE WILL BE OVER FOR THIS ROUND!\n\n";
            num_of_pl_thrown_out_from_voting = -1;
            return;
        }
        /*Though, in case this is the first time players are voting for this round, fill-up vector p with the numbers of the players who will now
        (in the second voting round) be the applicants for leaving the game.*/
        for(int i=0; i<total_number_of_players; i++){
            if(players[i].get_CurrentVotes() == m){
                p.push_back(i);
                cout << "player " << i << '\n';
            }
        }
        cout << "HAS/HAVE COLLECTED THE MOST VOTES. THIS IS WHY THE VOTING PROCEDURE HAS TO BE REPEATED, BUT THIS TIME\n"
             << "THE APPLICANTS FOR EXITING THE GAME WILL ONLY BE THE PLAYERS MENTIONED ABOVE.\n\n";
        voting(players, 2, count);   //call voting() function for the second time in the same round
    }
}


/*Function that finds the biggest one between the numbers of votes each player has collected and returns it.*/
int max(vector<player> &players){
    int max = players[0].get_CurrentVotes();
    for(int i=1; i<total_number_of_players; i++){
        if(players[i].get_CurrentVotes() > max){
            max = players[i].get_CurrentVotes();
        }
    }
    return max;
}

/*FUNCTION FILL_VECTOR_ROUNDS()*/
/*This function is for storing some specific information for each particular round, after it ends. Variable num_goodbye will have the number of player
that has been killed by gangster in this round (if gangster hasn't killed anybody, it will contain number -1). Variable num_of_pl_thrown_out_from_voting
will have the number of the player that was thrown outside the game as a result of the voting procedure (if the voting procedure has end with nobody leaving
the game, variable num_of_player_thrown_out_from_voting will contain number -1). We fill-up the vector 'round' with information for each round (name of the player
killed from gangster (if no player is killed , give as a parameter 'space' string), number of the particular game round and the name of the player who's been voted 
to leave the game (if no player has been voted to leave the game, give as a parameter 'space' string)). */
void fill_vector_rounds(vector<player> &players, int num_of_round, int num_goodbye, int num_of_pl_thrown_out_from_voting, vector<Round> &round){
    if(num_goodbye != -1 && num_of_pl_thrown_out_from_voting != -1){ 

        //we use push_back() function member each time in order to put an object of type Round into the vector. As argument, we use the constructor of class Round
        round.push_back(Round(players[num_goodbye].get_Name(), num_of_round, players[num_of_pl_thrown_out_from_voting].get_Name()));
    
    }else if(num_goodbye != -1 && num_of_pl_thrown_out_from_voting == -1){
        
        round.push_back(Round(players[num_goodbye].get_Name(), num_of_round, " "));
    
    }else if(num_goodbye == -1 && num_of_pl_thrown_out_from_voting != -1){

        round.push_back(Round(" ", num_of_round, players[num_of_pl_thrown_out_from_voting].get_Name()));

    }else{

        round.push_back(Round(" ", num_of_round, " "));

    }
}

/*The following function is called to 'tell' us if the game has reached to an end or not. It returns
number 1 if the game has to continue , or number 0 if the game has to be finished. We have variables 'gangster' firstly
initialized with 0. If the gangster is alive, put no.1 into variable gangster. If two players have remained,
one of which is the gangster, the return value should be number 0. If gangster is not alive and variable gangster remains
with number 0, the return value should also be number 0. In any other case, the return value should be a number other than 0 
(in this function, we use number 1).*/

int check_for_end_of_game(vector<player> &players, int x1){ 
    int gangster = 0;
    int i;
    if(players[x1].get_Alive() == true){
        gangster = 1; 
    }
    if(gangster == 0){
        cout << "THE CITIZENS WON AND THE GAME IS OVER!\n";
        return 0;
    }else if(current_number_of_players == 2){                                   // We check if the total number of players that have remained into 
                                                                   // the game is only 2 players, one of whom is the gangster for sure!
        cout << "THE GANGSTER WON AND THE GAME IS OVER!\n";
        return 0;
    }else{
        return 1;
    }
}

/*FUNCTION FILL_VECTOR_PLAYERS*/
/*This function is for filling up the vector with the players' information, that is obtained from the file that is opened.*/
int fill_vector_players(vector<player> &players, ifstream &ist){
    string name, role;
    int n = 0;
    while(ist >> name >> role){
        players.push_back(player(name, true, role));
        n++;
    }
    return n;
}
#include "std_lib_facilities.h"
#include <time.h>             // We will use time() function as argument of srand() function.

/*HERE ARE THE DECLARATIONS OF ALL THE FUNCTIONS USED IN OUR PROGRAMME*/
int initialization(vector<string> &players);
int random_choice(string, int, int, vector <string> &);
void init(vector<string> &);
void choice_menu(vector<string> &);
void night(int, vector<string>&);
void day(vector<string> &);
void voting(vector<string> &);
int max(vector<int> &);
void voting_results_check(vector<string> &, vector<int> &, int);
void voting_repetition(vector<string> &, vector<int> &, int);
int check_for_end_of_game(vector<string> &);

int A = -1;   /* Here we will be storing the number of the last player that left the game. We initialize it with
                 number -1 because we don't want the compiler to give it number 0 by default (because there is a player
                 with this number) So what is this variable's role ? Well, each time a particular player has to leave the game,
                 variable A is given the number of this player. It is used in choice_menu() function when the user chooses either
                 option 3 (in case nobody has left the game yet) */ 

int total_pl_num = 7; /*This variable will be used in order to show us, when needed , how many players have remained into the game.
                        It is firstly initiallized with number 7 (because when the game begins, players are 7 in total) and each time 
                        one player leaves, the value of the variable is decreased by one.*/


/* We create these classes for the exceptions below (in order to have something unique for keyword 'throw' each time we use it for 
a different reason). Class 'wrong_number' is for when the number a player gives as an input for the voting procedure doesn't align to
a player (it is either < 0 or > 6). Class 'player_already_out' is for when a player votes someone who exists as a player, but has already
left the game. Class 'not_one_of_those_with_the_maniest_votes' is for when the voting procedure is repeated in the same round [becase some players
(2 to 6 players) have drawn] and a player votes someone that, although he is into the game, didn't collect the maniest votes in the previous
voting procedure. Finally, class 'doc_cannot_visit_himself' is for when the doctor chooses to 'visit' himself, something that is not allowed
into the game.*/
class wrong_number{};  
class player_already_out{}; 
class not_one_of_those_with_the_maniest_votes{};
class doc_cannot_visit_himself{};

/*MAIN FUNCTION*/
int main(){   
    vector<string> players;  // In vector "players" we are going to store the role of each player
    cout << "THE GAME STARTS\n";
    init(players);           // In this function we initialize the values of vector players with 'space' character.
    int x1 = initialization(players);  // We give a role to each player (doctor, gangster or citizen) by  calling 
                                       // function "Initialization". This particular function returns (in var. x1) 
                                       // the number of the player who is going to be the gangster. 
    int b;                    // We will use this variable as a way to escape or remain into the do-while loop.
    do{
        choice_menu(players); // We want to show the menu each time the day zone ends, before the new night zone begins.
        night(x1, players);   // The NIGHT ZONE starts!
        day(players);         // The DAY ZONE starts, after the night zone has reached to an end!
        b = check_for_end_of_game(players); /* The following function is called to 'tell' us if the game has reached to an end or not. 
                                             It returns number 1 if the game has to continue , or number 0 if the game has to be finished.*/
    }while(b);         
    
    return 0;
}                    

/*INIT FUNCTION*/
// In this function we initialize the values of vector players with 'space' character.
void init(vector<string> &players){
    for(int i=0; i<7; i++){
        players.push_back(" ");
    }
}


/*INITIALIZATION FUNCTION*/
// We call this function to give a role (doctor, citizen or gangster) to each of the seven players in the game. It returns the number of 
// the player who has been randomly chosen to be the gangster.
int initialization(vector<string> &players){
    srand(time(0));                   // initialize the seed (the starting point for rand() function).
    int lower = 0, upper = 6;
    int x1 = random_choice("gangster", upper, lower, players);  // we store which player is going to be the gangster
    int x2 = random_choice("doctor", upper, lower, players); // (variable x1) and which player is going to be the doctor 
    for(int i=0; i<7; i++){                                  // (variable x2). Now we know which players are left, so as 
        if(i != x1 && i != x2){                              // to be given the role of citizen.
            players[i] = "citizen";
        }
    }
    return x1;    
}                                                           


/* Function for giving to one randomly chosen player the role of gangster (first call) and ti anither player (also randomly chosen)
the role of doctor (second call). */
int random_choice(string role, int upper, int lower, vector<string> &players){ // function for giving 
    int n = rand() % (upper - lower + 1) + lower;                         // to one randomly chosen player
    players[n] = role;                                          // the role of gangster (first call) and to 
    return n;                                      // another player (also randomly chosen) the role of doctor (second call).
}                                                  // This function returns who (which player) has been given the role of
                                                   // gangster (first call) or doctor (second call) each time.

/* CHOICE_MENU FUNCTION */
/*This function, when it is called by main function, shows a menu with 5 different options available for the user/player to choose from.
The player chooses the option he wants and gets the associated result as a message on the screen. This process is repeated again and again,
until the player chooses option 5, function choice_menu() returns void and the game continues as it is. The player has to give as an input 
one number between number 1, 2, 3, 4 or 5, each time the menu shows up. If he gives a number lesser than 1 or greater than 5, error 
'wrong_number' is occurred.*/
void choice_menu(vector<string> &players){
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
                for(int i=0; i<7; i++){
                    if(players[i] != "out of the game"){
                        cout << i << ' ';
                    }
                }
                cout << "\n\nselect one of the above options and type its number:\n\n";
            }else if(choice == 2){
                cout << "STILL IN THE GAME ARE PLAYERS:\n";
                for(int i=0; i<7; i++){
                    if(players[i] != "out of the game"){
                        cout << i << '(' << players[i] << ")  ";
                    }
                }
                cout << "\n\nselect one of the above options and type its number:\n\n";
            }else if(choice == 3){
                if(A == -1){
                    cout << "NOBODY HAS LEFT THE GAME YET.\n";
                }else{
                    cout << "THE PLAYERS WHO HAVE ALREADY LEFT THE GAME ARE PLAYERS:\n";
                    for(int i=0; i<7; i++){
                        if(players[i] == "out of the game"){
                            cout << i << ' ';
                        }
                    }
                }
                cout << "\n\nselect one of the above options and type its number:\n\n";
            }else if(choice == 4){
                if(A == -1){
                    cout << "NOBODY HAS LEFT THE GAME YET.\n";
                }else{
                    cout << "THE PLAYER WHO WAS THE LAST ONE TILL NOW TO LEAVE THE GAME IS PLAYER: ";
                    cout << A << "\n\nselect one of the above options and type its number:\n\n";
                }   
            }else{
                cout << "OK, LET'S MOVE ON ...\n";
            }
        }catch(wrong_number){
            cout << "WRONG CHOICE. TRY AGAIN:\n";
        }         
    }while(choice != 5);
}



// NIGHT ZONE begins!
void night(int x1, vector<string> &players){  
    cout <<"NIGHT ZONE BEGINS NOW!\nCITIZENS CLOSE YOUR EYES AND SLEEP!\n" 
        << "THE  GANGSTER IS PLAYER NUMBER " << x1 << "\n\n"
        << "GANGSTER GIVE THE NUMBER OF THE PLAYER YOU WANT TO LEAVE THE GAME:";
    int number_goodbye;
    int k = 0;
    do{                                    // We use an exception to 'catch' bad / wrong inputs.
        cin >> number_goodbye;             // we store the number of the player who the Gangster wants to leave   
        try{                               // the game.
            if(number_goodbye < 0 || number_goodbye > 6){   // If Gangster gave the number of a player who does
                throw wrong_number();                       // not exist ---> wrong_number error!
            }else if(players[number_goodbye] == "out of the game"){  // If Gangster gave the number of a player who
                throw player_already_out();                       // has already left the game ---> player_already_out error!
            }else{
                k = 1;  // k's value has to differ from zero (0) in order to 'escape' the loop and let the game continue.
            }
        }catch(wrong_number){
            cout << "INVALID NUMBER! PLEASE GIVE A NUMBEER BETWEEN 0 AND 6, WHICH WILL ALSO REFER TO A PLAYER"
                 << " WHO HAS NOT ALREADY LEFT THE GAME:\n"; // In case of wrong_number error, print these messages.
        }catch(player_already_out){
            cout << "THIS PLAYER IS ALREADY OUT OF THE GAME. PLEASE CHOOSE ANOTHER PLAYER AND GIVE HIS NUMBER:\n";
        }                                             // In case of player_already_out error, print the message above.
    }while(!k);    // Check if there was an error or not and decide if the game will continue or if the gangster has to give another value.
    for(int i=0; i<7; i++){
        if(players[i] == "doctor"){
            int k2 = 0;
            cout << "\n\nDOCTOR GIVE THE NUMBER OF THE PLAYER YOU ARE GOING TO 'VISIT' TONIGHT: ";
            int number_visit;
            do{
                cin >> number_visit;  // With the exact same way we check the validation of 
                try{                 // the value / number that doctor has given to us.
                    if(number_visit < 0 || number_visit > 6){
                        throw wrong_number();
                    }else if(players[number_visit] == "out of the game"){
                        throw player_already_out();
                    }else if(players[number_visit] == "doctor"){
                        throw doc_cannot_visit_himself();
                    }
                    else{
                        k2 = 1;  
                    }
                }catch(wrong_number){
                    cout << "\n\nINVALID NUMBER! PLEASE GIVE A NUMBEER BETWEEN 0 AND 6, WHICH WILL ALSO REFER TO A PLAYER"
                         << " WHO HAS NOT ALREADY LEFT THE GAME:\n";
                }catch(player_already_out){
                    cout << "\n\nTHIS PLAYER IS ALREADY OUT OF THE GAME. PLEASE CHOOSE ANOTHER PLAYER AND GIVE HIS NUMBER:\n";
                }catch(doc_cannot_visit_himself){
                    cout << "\n\nHEY DOCTOR, YOU CANNOT 'VISIT' YOURSELF! I'M SORRY!\nPLEASE, VOTE AGAIN!:\n";
                }
            }while(!k2);
            if(number_goodbye == number_visit){ // In case the player Gangster wants to leave and the player Doctor wants to visit
                cout << "\n\nNIGHT ZONE IS FINALLY OVER AND NOBODY IS LEAVING THE GAME!\n\n"; // are the same, nobody is leaving the game.
                return;
            }
        }
    }
    
    cout << "\n\nNIGHT ZONE IS FINALLY OVER, WITH PLAYER " << number_goodbye << " LEAVING THE GAME.\n\n";
    players[number_goodbye] = "out of the game";   // In other case, the player with the number the gangster has given leaves the 
    A = number_goodbye;                            // game and his number is given to variable A.
    total_pl_num -= 1;                         // A player is leaving, so variable total_pl_num has to be decreased by one.                    
}                                            // Finally ,the NIGHT ZONE is over.

/*DAY FUNCTION*/
/*After the night zone is over, DAY ZONE begins! After the messages have appeared on the screen, function day() calls function voting().*/
void day(vector<string> &players){
    cout << "DAY ZONE BEGINS NOW!\n\n"
         << "IT'S TIME FOR VOTING!\n\n\n";
    cout << "THE REMAINING PLAYERS ARE:\n";
    for(int i=0; i<7; i++){
        if(players[i] != "out of the game"){
            cout << "player number " << i << '\n';
        }
    }
    cout << '\n';
    int times = 0;
    voting(players);
}


/*VOTING FUNCTION*/
/*In this function the first voting phase is executed. Firstly, we need a place to store the number of votes each player has collected. This
is why we use a vector<int> (named: 'pl') with seven 'positions' (one for each player), which we initiallize with 0 in the beginning. Then, 
each remaining player votes whoever he wants to see leaving the game and the input's validity is checked through exceptions 'wrong_number' 
(case when the input is a number lesser than 0 or greater than 6) and 'player_already_out()' (case when the input is the number of a player that
has already left the game). After the voting procedure has been over, function voting() calls function voting_results_check() to check the 
voting results. This is the first time in a game round that function voting_results_check() is called, so for third argument it takes nunmber 1.*/
void voting(vector<string> &players){
    vector<int> pl(7);
    int x;
    for(int i=0; i < 7; i++){
        pl[i] = 0;
    }
    int j;
    for(int i=0; i<7; i++){
        if(players[i] != "out of the game"){
            cout << "PLAYER NUMBER " << i << " PLEASE VOTE WHO YOU WANT TO LEAVE THE GAME (type player's number):\n";
            j = 0;
            do{
                try{
                    cin >> x;
                    if(x<0 || x>6){
                        throw wrong_number();
                    }else if(players[x] == "out of the game"){
                        throw player_already_out();
                    }else{
                        pl[x] += 1;
                        j = 1;  
                    }                             
                }catch(wrong_number){
                    cout << "INVALID NUMBER! PLEASE GIVE A NUMBEER BETWEEN 0 AND 6, WHICH WILL ALSO REFER TO A PLAYER"
                         << " WHO HAS NOT ALREADY LEFT THE GAME:\n";
                }catch(player_already_out){
                    cout << "THIS PLAYER IS ALREADY OUT OF THE GAME. PLEASE CHOOSE ANOTHER PLAYER AND GIVE HIS NUMBER:\n";
                }
            }while(!j);
        }
    }
    cout << "THE VOTING IS OVER!\n";
    voting_results_check(players, pl, 1);
}


/*VOTING_RESULTS_CHECK FUNCTION*/
/*When called for the first time (from function voting()), it takes number 1 as its third argument. Instead, if it is called for the second time
(from function voting_repetition()), it takes number 2 as its third argument. This function calls function max() and we get what was the biggest 
number of votes that have been collected through the previously finished voting procedure. Then, we count how many players have collected that 
number of votes into the variable count. If we find out that it was only one player (if(count == 1)), he has to leave the game. The total_pl_num 
is decreased by one and the player's number is given to variable A. If we find out that all players have collected the exact same amount of votes
(if(count == total_pl_num)), nobody is leaving. In any other case, one of two things happen. If its third argument holds number 1 (if(times == 1)),
the voting procedure has to be repeated. For that reason, we call function voting_repetition(). But this time, applicants for exiting the game are
going to be only those players who were even in total amount of votes. Instead, if its third argument holds number 2 (if(times == 2)), the voting 
procedure is terminated and no player leaves the game, because we will have reached to a point when for the second time in a row, more than one p
players will have collected the same amount of 'max' votes.*/
void voting_results_check(vector<string> &players, vector<int> &pl, int times){
    int m = max(pl);   // function max() returns the biggest number of votes
    int count = 0;
    vector<int> p;
    for(int i=0; i<7; i++){
        if(pl[i] == m){
            count += 1;       // we check if there are more than one players who have collected the biggest number of votes
        }
    }
    if(count == 1){
        for(int i=0; i<7; i++){
            if(pl[i] == m){
                cout << "PLAYER " << i << " HAS COLLECTED THE MANIEST VOTES, SO HE HAS TO LEAVE THE GAME!\n\n";
                players[i] = "out of the game";
                A = i;
                total_pl_num -= 1;  // A player is leaving so the value of the variable total_pl_num has to be decreased by one.
                return;
            }
        }
    }else if(count == total_pl_num){  // In case all the remaining players (total_pl_num) have collected the same amount of votes.
        cout << "ALL PLAYERS HAVE COLLECTED THE SAME NUMBER OF VOTES. THIS IS WHY NOBODY LEAVES THE GAME FOR THIS ROUND.\n";
        return;
    }else{
        if(times == 2){
            cout << "THERE IS NOT ONE PLAYER WITH THE MANIEST VOTES FOR THE SECOND TIME IN A ROW, SO THIS IS WHY\n"
                 << "THE VOTING PROCEDURE WILL BE OVER FOR THIS ROUND!\n\n";
            return;
        }
        for(int i=0; i<7; i++){
            if(pl[i] == m){
                p.push_back(i);
                cout << "player " << i << '\n';
            }
        }
        cout << "HAS/HAVE COLLECTED THE MANIEST VOTES. THIS IS WHY THE VOTING PROCEDURE HAS TO BE REPEATED, BUT THIS TIME\n"
             << "THE APPLICANTS FOR EXITING THE GAME WILL ONLY BE PLAYERS MENTIONED ABOVE.\n\n";
        voting_repetition(players, p, count);
    }
}

/*VOTING_REPETITION FUNCTION*/
/*This function is called by voting_results_check() function in case more than one and less than total_pl_num players have collected 'max'
votes. In this function, we get as arguments (from left to right) the vector with the players' roles, the vector with the applicants' (for 
exiting the game) numbers (in order to know who they are now) and how many the applicants are going to be. We create a vector<int> named pl
(firstly initialized with 0 to all its 7 'positions') in order to collect the number of votes each applicant will collect. For practical reasons,
the vector has by default 7 'positions', but the 'positions' of the players who are not applicants anymore will be holding the value 0 for the 
whole voting procedure. We have the same two errors as in most functions above ('wrong_number' and 'player_already_out'), but we now also have 
error named: 'not_one_of_those_with_the_maniest_votes'. If variable flag remains holding number 0 as the programme is exiting from for loop of
line 353(which is into try{} block), that means that the number the player who just voted has given as input to the programme is neither invalid,
nor of a player who is already out, but it's the number of a player who is not applicant for exiting the game for this voting round. In the end,
function voting_results_check() is called to find out the final voting results.*/
void voting_repetition(vector<string> &players, vector<int> &p, int count){
    vector<int> pl(7);
/*  for(int i=0; i<7; i++){
        pl[i] = 0;
    }*/
    int x,j = 0;
    int flag = 0;
    for(int i=0; i<7; i++){
        if(players[i] != "out of the game"){
            cout << "PLAYER NUMBER " << i << " PLEASE VOTE WHO YOU WANT TO LEAVE THE GAME (type player's number):\n";
            do{
                try{
                    cin >> x;
                    if(x<0 || x>6){
                        throw wrong_number();
                    }else if(players[x] == "out of the game"){
                        throw player_already_out();
                    }
                    for(int i=0; i < count; i++){
                        if(x == p[i]){
                            pl[x] += 1;
                            flag = 1; 
                        }
                    }
                    if(flag == 0){
                        throw not_one_of_those_with_the_maniest_votes();
                    }                  
                    j = 1;            
                }catch(wrong_number){
                    cout << "INVALID NUMBER! PLEASE GIVE A NUMBEER BETWEEN 0 AND 6, WHICH WILL ALSO REFER TO A PLAYER"
                         << "WHO HAS NOT ALREADY LEFT THE GAME:\n";
                }catch(player_already_out){
                    cout << "THIS PLAYER IS ALREADY OUT OF THE GAME. PLEASE CHOOSE ANOTHER PLAYER AND GIVE HIS NUMBER:\n";
                }catch(not_one_of_those_with_the_maniest_votes){
                    cout << "YOU CAN NOT VOTE THIS PLAYER. THE VOTING NOW IS BETWEEN ONE OF THE FOLLOWING PLAYERS:\n";
                    for(int i=0; i < count; i++){
                        cout << "player " << p[i] << ' ';
                    }
                    cout << "\nSO PLEASE VOTE AGAIN:\n";
                }
            }while(!j);
        }
    }
    cout << "THE VOTING IS OVER!\n\n";
    voting_results_check(players, pl, 2);
}



/*Function that finds the biggest one between the numbers of votes each player has collected and returns it.*/
int max(vector<int> &pl){
    int max = pl[0];
    for(int i=1; i<7; i++){
        if(pl[i] > max){
            max = pl[i];
        }
    }
    return max;
}



/*The following function is called to 'tell' us if the game has reached to an end or not. It returns
number 1 if the game has to continue , or number 0 if the game has to be finished. We have variables 'gangster' and 
'total_number' firstly initialized with 0. We check the value of each player (for loop) in order to see how many of 
them have remained into the game (whenever we find one remaining player, we increase the value of 'total_number' by 
one). If the gangster has left the game, the return value should be number 0. Also, if two players have remained, 
one of which is the gangster, the return value should also be number 0. In any other case, the return value should be 
number 1 (not 0 in any case). */

int check_for_end_of_game(vector<string> &players){ 
    int gangster = 0;
    for(int i=0; i<7; i++){
        if(players[i] == "gangster"){
            gangster = 1; 
        }
    }
    if(gangster == 0){
        cout << "THE CITIZENS WON AND THE GAME IS OVER!\n";
        return 0;
    }else if(total_pl_num == 2){                                   // We check if the total number of players that have remained into 
                                                                   // the game is only 2 players, one of whom is the gangster for sure!
        cout << "THE GANGSTER WON AND THE GAME IS OVER!\n";
        return 0;
    }else{
        return 1;
    }
}
